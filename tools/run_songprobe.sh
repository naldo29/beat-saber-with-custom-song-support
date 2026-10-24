#!/bin/bash
# Run with bash. --check is read-only; --run builds/installs a separate test app.
set -euo pipefail
cd "$(dirname "$0")/.."

check_mac() {
  [ "$(uname -s)" = Darwin ] && [ "$(uname -m)" = arm64 ] || {
    echo "This build needs an Apple silicon Mac." >&2; exit 1;
  }
  for command in xcodebuild xcrun python3 apktool pkg-config; do
    command -v "$command" >/dev/null || {
      echo "Missing $command. See docs/custom-songs-port.md." >&2; exit 1;
    }
  done
  xcrun --sdk xros --show-sdk-path
  xcodebuild -version
  xcrun devicectl list devices
  echo "Mac tools found. The device must be paired, awake and in Developer Mode."
}

case "${1:---check}" in
  --check) check_mac; exit 0 ;;
  --run) [ "$#" = 3 ] || {
    echo 'Usage: bash tools/run_songprobe.sh --run /path/to/game.apk /path/to/main.1716.com.beatgames.beatsaber.obb' >&2
    exit 2
  } ;;
  *) echo 'Use --check or --run APK OBB.' >&2; exit 2 ;;
esac
check_mac
# Resolve before changing any source paths; the helper is run from the repo root.
APK_SOURCE=$(python3 -c 'import pathlib,sys; print(pathlib.Path(sys.argv[1]).resolve(strict=True))' "$2")
OBB_SOURCE=$(python3 -c 'import pathlib,sys; print(pathlib.Path(sys.argv[1]).resolve(strict=True))' "$3")
[ "$(basename "$OBB_SOURCE")" = main.1716.com.beatgames.beatsaber.obb ] || {
  echo "Expected the versionCode 1716 OBB filename." >&2; exit 1;
}

# Never replace an existing different APK or decoded tree.
if [ -f beatsaber-1.40.apk ]; then
  cmp -s "$APK_SOURCE" beatsaber-1.40.apk || {
    echo "A different beatsaber-1.40.apk exists. Use a fresh extracted port folder." >&2; exit 1;
  }
elif [ -e beatsaber140 ]; then
  echo "A decoded tree without its source APK exists. Use a fresh extracted port folder." >&2
  exit 1
else
  cp "$APK_SOURCE" beatsaber-1.40.apk
fi
if [ ! -d beatsaber140 ]; then
  apktool d -o beatsaber140 beatsaber-1.40.apk
fi
grep -Eq 'versionName:.*1\.40\.8_7379' beatsaber140/apktool.yml &&
grep -Eq "versionCode:.*1716" beatsaber140/apktool.yml || {
  echo "This first probe is for Beat Saber 1.40.8_7379 / versionCode 1716." >&2; exit 1;
}

mkdir -p probe-results
SESSION_DIR=$(mktemp -d "$PWD/probe-results/session-XXXXXXXX")
echo "Build and device logs: $SESSION_DIR"
export KLEPTON_BUNDLE_ID="dev.klepton.target.beatsaber140.songprobe.test"
export KLEPTON_DISPLAY_SUFFIX=" (Song Probe)"
export KL_SONG_PROBE=1
export KL_SONGS=0
export KL_OBB_DIR="$(dirname "$OBB_SOURCE")"
export KL_LOG_OUT="$SESSION_DIR/headset.log"
# The unpacked archive carries upstream shell files as executable files.
# No existing game library, save directory or installed app is repurposed.
{
  make mvk
  make angle-all
  make check TARGET=beatsaber140
  ./build_run_vpro.sh beatsaber140 --stage --frames 1800
} 2>&1 | tee "$SESSION_DIR/build-and-run.log"
echo "Probe output (if the game reached its frame loop):"
if [ -f "$KL_LOG_OUT" ]; then
  grep -aF '[song-probe]' "$KL_LOG_OUT" || true
fi
echo "Keep this session folder for the next implementation step."
