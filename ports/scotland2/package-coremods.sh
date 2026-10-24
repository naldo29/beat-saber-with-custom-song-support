#!/bin/bash
# Add the three remaining original core mods to the working custom-song stack.
set -euo pipefail
cd "$(dirname "$0")/../.."
PLATFORM="${1:-xros}"
SC2_TEST_SET=songdownloader bash ports/scotland2/package.sh "$PLATFORM"
BUNDLE="$PWD/build/scotland2/$PLATFORM-coremods-device/bundle"
mkdir -p "$BUNDLE";cp -R "build/scotland2/$PLATFORM-songdownloader-device/bundle/." "$BUNDLE/"
META="$BUNDLE/Scotland2Metadata"
for name in datakeeper fakeid unicode;do
 python3 ports/scotland2/tools/build_remaining_core.py "$name"
 case "$name" in fakeid) lib=libFakeId;; *) lib=lib$name;; esac
 phase=early_mods;if [ "$name" = unicode ];then phase=mods;fi
 cp "build/scotland2/android-$name/$lib.so" "$META/$phase/"
 cp "build/scotland2/android-$name/hook-profile.txt" "$META/profiles/$name-hooks.txt"
 bash ports/scotland2/stage-mod-framework.sh "$PLATFORM" "$META/$phase/$lib.so" "$BUNDLE/Frameworks"
done
python3 - "$META" <<'PY'
from pathlib import Path
import json,sys
p=Path(sys.argv[1])/'provenance.json';d=json.loads(p.read_text());d['testSet']='coremods'
for name in ['datakeeper','fakeid','unicode']:d[name]=json.loads(Path('build/scotland2/android-'+name+'/provenance.json').read_text())
d['scope']='All eleven original core mods built; actual physical remaining-mod checks pending.';p.write_text(json.dumps(d,indent=2)+'\n')
PY
if [ "$PLATFORM" = xros ];then rm -rf visionos/Scotland2Metadata;cp -R "$META" visionos/Scotland2Metadata;fi
