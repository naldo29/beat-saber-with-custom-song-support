#!/bin/bash
# Build/sign an UPDATE to the existing test app. Does not install or launch.
set -euo pipefail
cd "$(dirname "$0")/../.."
export KLEPTON_TARGET=beatsaber140 XROS_PLATFORMS=xros
export KLEPTON_BUNDLE_ID=dev.klepton.beatsaber140.scotland2.test
export KLEPTON_DISPLAY_SUFFIX=' (Scotland2 Test)'
export KLEPTON_EVA=0 KLEPTON_LOW_LATENCY=0 KLEPTON_SCOTLAND2=1
make build/klepton-ld xros XROS_PLATFORMS=xros
bash ports/scotland2/package.sh xros
(
  cd visionos
  bash mkguest.sh
  bash mkangle.sh
  bash mkmvk.sh
  if [[ "${SC2_TEST_SET:-tinyxml2}" = hooks || "${SC2_TEST_SET:-tinyxml2}" = customtypes || "${SC2_TEST_SET:-tinyxml2}" = lapiz || "${SC2_TEST_SET:-tinyxml2}" = bsml || "${SC2_TEST_SET:-tinyxml2}" = songcore ]]; then bash ../ports/scotland2/prepare-game-hooks.sh; fi
  python3 gen_xcodeproj.py
  xcodebuild -project Klepton.xcodeproj -scheme Klepton -configuration Debug \
    -destination 'generic/platform=visionOS' -derivedDataPath build/dd-device-beatsaber140 \
    -allowProvisioningUpdates build
)
APP=visionos/build/dd-device-beatsaber140/Build/Products/Debug-xros/Klepton.app
codesign --verify --deep --strict "$APP"
python3 - "$APP" <<'PY'
import os,plistlib,sys
from pathlib import Path
app=Path(sys.argv[1])
info=plistlib.loads((app/'Info.plist').read_bytes())
assert info['CFBundleIdentifier']=='dev.klepton.beatsaber140.scotland2.test'
assert str(info['KLScotland2Test'])=='1'
for phase in ['libs','early_mods','mods','shims']:
 assert (app/'Scotland2Metadata'/phase).is_dir(),phase
songcore=os.environ.get('SC2_TEST_SET')=='songcore'
bsml=os.environ.get('SC2_TEST_SET') in ('bsml','songcore')
lapiz=os.environ.get('SC2_TEST_SET') in ('lapiz','bsml','songcore')
ct=os.environ.get('SC2_TEST_SET') in ('customtypes','lapiz','bsml','songcore')
hooks=os.environ.get('SC2_TEST_SET') in ('hooks','customtypes','lapiz','bsml','songcore')
paper=os.environ.get('SC2_TEST_SET','tinyxml2') in ('paper','hooks','customtypes','lapiz','bsml','songcore')
expected={'libs/libtinyxml2.so'} | ({'libs/libpaper2_scotland2.so'} if paper else set())
if hooks:
 assert str(info['KLScotland2Hooks'])=='1'
 expected|={'libs/libbeatsaber-hook.so','shims/libsl2_abi.so','shims/libsc2_flamingo.so','shims/libsc2_flamingo_probe.so'}
if ct:
 assert str(info['KLScotland2CustomTypes'])==('0' if lapiz else '1')
 expected|={'libs/libcustom-types.so','shims/libsc2_customtypes_probe.so'}
if lapiz:
 assert str(info['KLScotland2Lapiz'])==('0' if bsml else '1')
 expected|={'early_mods/liblapiz.so'}
 assert (app/'Scotland2Metadata/profiles/lapiz-hooks.txt').is_file()
if bsml:
 assert str(info['KLScotland2BSML'])=='1'
 expected|={'early_mods/libbsml.so','shims/libsc2_bsml_probe.so'}
 assert (app/'Scotland2Metadata/profiles/bsml-hooks.txt').is_file()
if songcore:
 assert str(info['KLScotland2SongCore'])=='1'
 expected|={'mods/libsongcore.so','shims/libsc2_songcore_probe.so'}
 assert (app/'Scotland2Metadata/profiles/songcore-hooks.txt').is_file()
assert {str(p.relative_to(app/'Scotland2Metadata')) for p in (app/'Scotland2Metadata').rglob('*.so')}==expected
for name in ['Scotland2Core','libtinyxml2'] + (['libpaper2_scotland2'] if paper else []) + (['libbeatsaber-hook','libsl2_abi','libcxx_shared','libsc2_flamingo','libsc2_flamingo_probe'] if hooks else []) + (['libcustom-types','libsc2_customtypes_probe'] if ct else []) + (['liblapiz'] if lapiz else []) + (['libbsml','libsc2_bsml_probe'] if bsml else []) + (['libsongcore','libsc2_songcore_probe'] if songcore else []):
 assert (app/'Frameworks'/(name+'.framework')/name).is_file()
print('Verified same test-app bundle identifier, loader frameworks, and selected dependency metadata:', sorted(expected))
PY
echo "Built and signed, not installed: $APP"
