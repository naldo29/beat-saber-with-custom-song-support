# Custom-song developer build

## Verified baseline

Klepton base commit: `0333512150a91a9ac83ceeae6e89c2d9ddc44eb3`.
Beat Saber: `1.40.8_7379`. Physical test: October 24, 2026.
The user confirmed playback of a downloaded song and additional downloads;
the log corroborated original SongCore loads of 1, 2 and 3 songs and a normal
background exit, with no native fault marker. This is a tested source port,
not an independent replacement for SongCore or SongDownloader.

## Layout

- `runtime/guest/kl_scotland2.*`: native loader bridge and lifecycle integration.
- `runtime/guest/kl_aot_hooks.*`: dispatch through prepared, signed hook entries.
- `ports/scotland2/`: adapted loader, original mod sources, patches, probes and builders.
- `ports/scotland2/tools/prepare_hooks.py`: validates original game hashes and
  instructions, then prepares hook gates before Apple code signing.
- `port-deps/sources/*lock.json`: original dependency versions and checksums.
- `port-results/coremods/coremods-lock.json`: public package/dependency inventory.

## Build inputs

An Apple-silicon Mac, Xcode with the visionOS platform, a paired headset in
Developer Mode, and an Apple signing team are needed for device installation.
Supply your own matching game under `beatsaber140/` and `beatsaber-1.40.apk`,
following the original Klepton build guide. Game assets and OBB data are staged
into the app's Documents separately from its executable bundle.

The local port uses Android NDK r27c and the dependencies recorded in the lock
files (including the matching IL2CPP headers, generated bs-cordl SDK, fmt,
Capstone, mod SDK dependencies and the original released support libraries).
Those downloaded trees and toolchains are not vendored here. The source snapshot
contains the adapted mod trees, but does not yet automate every external input
needed on a clean Mac. Do not interpret the commands below as a complete fresh
installation guide.

Once the pinned dependencies and game inputs are present, the tested composition
is selected with `SC2_TEST_SET=songdownloader`. The development sequence is:

1. Build `build/klepton-ld` and `make XROS_PLATFORMS=xros xros`.
2. `SC2_TEST_SET=songdownloader bash ports/scotland2/package.sh xros`.
3. Set `KLEPTON_TARGET=beatsaber140`, `KLEPTON_SCOTLAND2=1`,
   `SC2_TEST_SET=songdownloader`, `KLEPTON_EVA=0`, `KLEPTON_LOW_LATENCY=0`,
   and your own `KLEPTON_TEAM` and unique `KLEPTON_BUNDLE_ID`.
4. In `visionos/`, run `mkguest.sh`, `mkangle.sh`, `mkmvk.sh`; then from the
   repository root run `SC2_TEST_SET=songdownloader bash ports/scotland2/prepare-game-hooks.sh`.
5. Generate the Xcode project with `visionos/gen_xcodeproj.py`, build/sign the
   physical visionOS target, install with Xcode/Device Hub, and stage game data
   with the original Klepton asset workflow using the same bundle identifier.

`ports/scotland2/build-test-app.sh` is the historical local test helper and uses
a fixed test bundle ID. It is not the public installer entry point. Keep the
same bundle ID when updating your own installation to retain its data.

## Why APK injection alone is insufficient

Android mods normally patch executable code at runtime. The device port prepares
selected game entry points before signing, and directs hooks through writable
pointers at runtime. Several original mods embed their hook engines, requiring
source rebuilding against the adapted shared backend. Native runtime support
and prepared game frameworks belong in the visionOS app bundle, not just inside
the Android APK. A future local patcher can automate this complete process.

## Important shared fixes

The port preserves original mod functionality while adapting common hook APIs,
Android filesystem/C++ behavior and module data sharing. Specific corrections
include standalone TLS certificate trust, a measured MetaCore native Unity
cleanup target, and SongCore's empty selection guard.

For nonempty song scans, Unity's OBB audio-bank index deliberately has no Unreal
base path. Directory synthesis must decline that empty base. Without this guard
a synthetic OBB handle could reach native `readdir` and crash after download.
`ports/scotland2/test-songcore-directory-bridge.sh` tests the actual SongCore
collector on main/worker threads with OBB indexing, plus a positive UE4 listing.

Historical development notes under `ports/scotland2` describe earlier stages;
the verified scope at the top of this document supersedes their pending-status
statements. Further gameplay and persistence testing remains necessary.

## Selected ten-mod composition

`SC2_TEST_SET=coremods` packages ten original core mods, excluding Fake ID.
Klepton already supplies the adult age-category response. The original eleven-mod
QMOD inventory and unused source port remain as historical references; Fake ID
is absent from active packaging, framework embedding, hook preparation and tests.

The selected update is installed in the existing test app. The user confirmed
menu startup and custom-song playback after Fake ID removal. Physical checks
cover the eight hooks from PlayerDataKeeper and Unicode, actual Unity texture
write/readback, original TMP CJK/emoji caching, and four custom songs loaded.
Earlier physical checks verified a real save backup byte-for-byte and repeated
song downloading. These results do not establish long-session or all-map support.

- PlayerDataKeeper0.2.5: original WriteAllText and File.Replace hooks back up
  normal and local save files, omit temporary files and restore them through
  original setup; tested with actual IL2CPP save APIs in an isolated fixture.
  The shared loader now reports the application's existing /files directory,
  matching its JNI paths. The upstream one-time lighting reset is disabled to
  preserve existing effects preferences. No saves are moved. A real headset
  backup was copied and compared byte-for-byte against the current player save;
  both were identical. Destructive save/restore tests run only in the isolated
  host fixture; normal original startup restoration also runs on the headset.
- Unicode0.1.7: original TMP hooks, style keys, glyph cache and texture atlas
  retained. Android platform font drawing is adapted to Apple CoreText through
  a flat C ABI. Tests exercise real CJK and color-emoji glyphs through both
  native and Android-module paths; styles, whitespace and invalid scalars are
  checked. The Apple path uploads to the original atlas with retained
  `Texture2D.SetPixels32` and `Apply` methods, avoiding the stripped
  `Graphics.CopyTexture_Region` icall. Host checks verify those methods exist;
  the on-device diagnostic passed a real Unity texture-region write/readback and
  repeat lookups through the original TMP glyph cache for CJK and color emoji.

Run `bash ports/scotland2/test-remaining-core.sh` only in the prepared developer
environment. Its save/restore fixture is host-only; never invoke that diagnostic
on a user's live game data. On-device observers verify eight added hooks and
schedule the actual Unicode/TMP glyph-cache check.

The current generator still guards the existing local test bundle identifier.
A generic installer with recipient-specific bundle IDs and automatic dependency
setup is separate unfinished work; these are developer build notes.
