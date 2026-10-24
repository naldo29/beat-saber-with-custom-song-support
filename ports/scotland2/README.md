> Current status: original SongDownloader downloads and SongCore custom-song playback
> were confirmed on physical Vision Pro on October 24, 2026. See [CUSTOM-SONGS.md](../../CUSTOM-SONGS.md).
> The notes below retain development history and may describe earlier incomplete stages.

# Scotland2 Apple loader and dependency port

This ports **the actual Scotland2 loader implementation**, not a replacement
custom-song loader. The original transferred sources remain unchanged in
`../../Beat-Saber-Mac-handoff/scotland2-research`. The working copy in
`upstream/` is based on commit `432f67bca61e822a87b27c001bddcd1061d25bf4`.
Its license is retained. `upstream.patch` records the source edits with Unix
line endings; `src/` contains the new Apple adapters.

## Latest dependency checkpoint

Actual Lapiz 0.2.23 source now builds with all its original public Lapiz exports.
Its original Scotland2 early callback passed on the Mac: 551 imports, zero
missing; 31 game targets installed through signed prepared entries; twenty Lapiz
types and two custom-types delegate wrappers registered; actual game name/type
lookup and scheduler allocation/Tick passed; repeat registration added no types.
Compiler-emitted `.sc2_hooks` records bind hook addresses to the original game
ELF and are validated before code signing. Actual SDK InstallHook followed by
two InstallOrigHook insertions also passes with a shared HookTracker. Mixed
untracked/normal/original ordering across multiple real mods is not yet verified.

Run `bash ports/scotland2/test-lapiz.sh` for the host composition. The separate
`test-import-rejection.sh` fixture has an unresolved import and a trapping
constructor: Scotland2 now rejects it before constructor execution/registration,
including recursive loads during its scoped module opens. Ordinary game image
loading retains its existing policy; host Unity inspection still explicitly
allows its one existing, unused `ptrace` import.

`SC2_TEST_SET=lapiz bash ports/scotland2/build-test-app.sh` builds only an update
to `dev.klepton.beatsaber140.scotland2.test`. That update is installed on the
physical Vision Pro with data retained and passed its real callback test:
31 installed hooks, twenty Lapiz types, scheduler allocation/Tick and the
original scene/late phase. The user confirmed the menu works. Evidence:
`port-results/headset-lapiz-20261023/boot-weak.log`. Actual BSML 0.4.55 now builds and its early callback passes on the Mac:
2,314 imports, zero missing; thirteen game hooks; 61 registered types with actual
name/type lookup and repeat-registration checks; real configuration writing.
Its pinned original assets, CSS parser, GIF reader and gif-lib are retained.
`test-bsml.sh` also verifies a platform replacement for beatsaber-hook's original
shell-based mkpath, installed at its signed prepared entry: spaces, bounded
string views, aliases and pre-existing directories pass; files, empty paths and
embedded NULs fail. The general system() refusal is unchanged. Evidence:
`port-results/session-20261023/bsml-verified.log`.
`SC2_TEST_SET=bsml bash ports/scotland2/build-test-app.sh` prepares the same
test app with original BSML early/late callbacks and the signed filesystem
helper. Its independent lifecycle consumer checks both original GameObjects
and components, the coroutine Awake singleton, and schedules a callback through
BSML's actual Unity Update. Physical BSML passed all those checks; the user confirmed the menu with a
visible Mods section. Evidence: `port-results/headset-bsml-20261024/boot-closed.log`.
SongCore's corrected original late callback now passes on the physical headset:
33 installed hooks, 23 types, actual loader service and scan completed (0 songs).
User confirms menu and Refresh Songs. Browsing/downloading and playback remain
unverified. Evidence: `port-results/headset-songcore-20261024/boot-generic-reconnected.log`. Actual SongCore 1.1.24 source now builds with 32 compiler-resolved
hook targets plus its manual runtime generic LRUCache_Add target (33 total) and its original public exports. Its exact Crypto++ 8.5.0 archive
and pinned headers are retained. `SC2_TEST_SONGCORE=1 bash ports/scotland2/test-bsml.sh`
checks the actual DSO through strict module loading: 3,015 imports, zero missing;
constructors and setup run, producing its real config. This manually opens the
module after the early callbacks; it does not exercise actual Scotland2 late
phase ordering or SongCore late_load/song playback. The first physical SongCore
run aborted because the generic target was missing from its signed preparation.
`generic-target-profile.json` binds the measured target to exact game/metadata
hashes; actual host reflection independently resolves the same address/word.
The corrected physical update passes startup, service initialization and an
empty scan. Its log also contains a nonfatal empty-level-selection exception,
which remains under investigation.

`test-weak-symbols.sh` verifies that scoped Scotland2 module opens share prior
default-visible weak/unique ELF OBJECTs and their initialization guards. Ordinary
game loads, functions and protected/hidden objects retain local binding. This is
a targeted data-sharing fix, not full Android namespace/symbol-preemption support.
Its before/after fixture proves one shared variable and one initialization across
two Android DSOs. Actual SongCore repeated 259 BSML field/method initializations
before the fix and zero after; all Mac module regressions still pass. Evidence:
`weak-symbols-verified.log` and `songcore-weak-verified.log`. Sections below include historical milestones.

## Implemented

- Compile upstream `loader.cpp` and `modloader.cpp` with Apple's C++ library.
- Keep Scotland2's dependency ordering, loaded-object registry, C queries,
  setup/load/late_load callbacks and their one-time invocation rules.
- Route executable loading and symbol ownership checks through a C callback
  boundary into Klepton, using embedded translated/signed Mach-O code.
- Read original ARM64 ELF dependency metadata as data with bounds checks.
  This does not require Android's linker namespaces or writable signed text.
- Fail the platform phase when a library fails to open, rather than reporting
  a successful bootstrap after Scotland2 merely logged the failure.
- Interpose Unity's guest `dlsym(handle, "il2cpp_init")` in the opt-in runtime.
  Call the actual initialization first, verify `il2cpp_domain_get`, then dispatch
  Scotland2's early-load phase. This target uses Unity 2022.3.33f1's **void**
  initialization API; its return register is not a domain pointer.
- Expose the real loader C API to guest relocations and `dlopen("libsl2.so")`.
  Configure writable files/external paths separately from read-only metadata.
- Run the released Paper 4.6.4 Rust logger through that C API. Its actual
  constructor and background worker write main and per-mod context logs.
- Provide seven C++ path/string getters through a separately compiled Android
  guest adapter, with Google's NDK r27c `libc++_shared.so`. Native Darwin C++
  objects never cross that boundary. The adapter is host-tested and included in the optional hooks device build.
- Link the exact Capstone archive from Scotland2's QPM dependency manifest into
  the guest adapter for the beatsaber-hook test. Keep its C structures and
  implementations in the same guest ABI domain as the released mods.

## Verified on this Mac

- Four core test scenarios cover dependency ordering, deduplication, phase
  ordering, setup/load/late_load timing, C result queries, optional exports,
  foreign-symbol rejection, dlopen failure, and malformed metadata.
- The new dependency parser matches the independent Python ELF audit for all
  17 actual supplied/transitive mod libraries, and rejects 11 bad inputs.
- The actual tinyxml2 10.0.0 QMOD library loads through the original Scotland2
  library phase and a signed Klepton translation. All 58 imports resolve.
  The exported `tinyxml2::XMLUtil::ToInt` executes successfully.
- The actual game's translated IL2CPP library initializes through the new
  dynamic loader bridge and reports 145 assemblies. The test bundle has **no
  early mods**; actual mod callback timing is tested separately with fixtures.
- Both the native core and the shipping runtime compile for physical visionOS.
- Paper resolves all 73 import relocations and writes both expected log files.
  The runtime observes the actual file contents before printing `PAPER_LOG_OK`.
- A separate signed Android consumer reads all seven C++ getters, copies and
  mutates returned path/string copies, and checks that the originals stay intact.
  Tests include long paths and the short `libil2cpp.so` path. Getter exports are
  audited; no Darwin C++ functions are used as aliases.
- With the guest Android runtime and original Capstone, actual beatsaber-hook
  6.4.2 initializes with **203 import relocations bound, zero missing**. Its real
  Capstone branch decoder executes successfully. The same dependency composition
  also permits the actual game's IL2CPP initialization and 145 assemblies.
  Its original executable trampoline-pool request is **rejected**, so this does
  not establish functional hooks. See `HOOK-PORT.md`.

## Not implemented or established

This is **not a complete Scotland2 port**. Actual Lapiz and BSML run on the
physical headset, including BSML Unity lifecycle/UI. SongCore is installed for
its first original late-callback test; the complete supplied mod stack and
custom-song playback remain unverified. No replacement song implementation
was added. The following paragraphs retain earlier milestone limits.

The loader core currently uses Darwin `std::__1`. Released Android mods import
some Scotland2 C++ APIs using `std::__ndk1`; those are not ABI-compatible.
The C export bridge and writable path configuration are implemented. Seven
path/string getters have a real guest C++ adapter tested on the Mac, but the
complete C++ registry/result-object API is not bridged. The default test package
admits tinyxml2; `SC2_TEST_SET=paper` additionally admits Paper. No actual early or late
mods are admitted. The optional hooks build packages beatsaber-hook and the
Android adapters; its physical execution is a separate verification step.
The game-library path is currently the virtual `libil2cpp.so` basename. File
consumers such as beatsaber-hook's `getLibil2cppSize` are not verified by the
object-compatibility test and still need a real file view or a source adapter.

The actual Flamingo installer and And64 public API now have source adapters
using signed gates and original-call trampolines prepared offline. Mac tests
cover chains, original calls, uninstall and the original scene lifecycle with a
fixture. Both adapters also hook the actual game's IL2CPP domain getter. See
`HOOK-PORT.md` for evidence and limits. Released hidden/inlined And64 copies
still require rebuilding/adaptation. The physical Unity scene hook passed; custom-type registration is host-tested and real dependent-mod callbacks remain open.

The visionOS build was later installed as an update to the same existing test
app on the physical Vision Pro. Both `LIBRARIES_OK` and `IL2CPP_PHASE_OK` were
captured directly from its live console. Frame submission and continuing
frame-pump heartbeats followed. The early-mod directory is empty. The user
confirmed that this loader-enabled build reached the main menu. The log repeats
an `Invalid texture unit` graphics warning whose relationship to the loader is
unproven; menu success does not establish that all rendering is correct.
The user had already confirmed that the earlier baseline copy worked.

A later update to that same app added Paper. The physical headset logged
`PAPER_READY`, `PAPER_LOG_OK`, `LIBRARIES_OK` and `IL2CPP_PHASE_OK`; the user
again confirmed normal menu access. Game data was retained by this update.
The capture continued for about nine minutes and later ended with signal 9.
The user explicitly confirmed closing the test app after the menu check; this
is consistent with the termination. No matching app crash report was found in
the archived diagnostic listing. Do not count that closure as a mod-loader
crash. Evidence is under
`port-results/headset-paper-20261023/`.

## Commands

From the Klepton repository root:

```sh
python3 ports/scotland2/tests/run.py
bash ports/scotland2/test-signed-library.sh
bash ports/scotland2/test-il2cpp.sh
bash ports/scotland2/test-paper.sh
bash ports/scotland2/test-android-paths.sh
SC2_TEST_BSHOOK=1 bash ports/scotland2/test-android-paths.sh
KL_SCOTLAND2_ABI=1 KL_GUEST_JIT=0 \
  SC2_USE_PREPARED_BUNDLE="$PWD/build/scotland2/macosx-bshook/bundle" \
  bash ports/scotland2/test-il2cpp.sh
bash ports/scotland2/build-test-app.sh
SC2_TEST_SET=paper bash ports/scotland2/build-test-app.sh
SC2_TEST_FLAMINGO=1 bash ports/scotland2/test-android-paths.sh
SC2_TEST_SET=hooks bash ports/scotland2/build-test-app.sh
```

The app commands build/sign only. They never install or launch a device app.
It uses the existing identifier
`dev.klepton.beatsaber140.scotland2.test`, with `KLEPTON_EVA=0` and
`KLEPTON_LOW_LATENCY=0` for the user's **free development account**.
Keep using that same identifier; the user has a three-app limit. Do not create
another test app identifier.

`KLEPTON_SCOTLAND2=1` embeds the native core, selected translated dependencies, and their
read-only ELF metadata. Project generation rejects this option for any other
app identifier. The app's `KLScotland2Test` setting enables the loader and
disables the old custom-song prototype. `KL_SCOTLAND2=0` permits a baseline
comparison using the same binary.

Expected device milestones are `[scotland2] LIBRARIES_OK` followed by
`[scotland2] IL2CPP_PHASE_OK`. They establish only the limited scope above.
The headset menu check has passed with both tinyxml2 and Paper. Logs and
provenance are in `port-results/session-20261023/` and the two headset folders.

The ABI test uses the project-local NDK under `port-deps/toolchains/`.
`ndk-lock.json` records Google's package URL and verified published SHA-1,
plus the locally computed SHA-256. The matching r27c identifier was found in
the supplied beatsaber-hook binary. Capstone's release archive and pinned
source/license are under `port-deps/sources/capstone-qpm/`; Paper's pinned
source and hash record are under `port-deps/sources/paperlog*`.

The pinned fmt 11.0.2 headers need a small `FMT_USE_CONSTEVAL` override guard for
Apple clang 21. The exact patch is in `fmt-apple.patch`; the downloaded archive's
hash and source URL are retained under `port-deps/sources/fmt-lock.json`.

## Physical hooks update

The same test app now includes both source hook adapters and prepared game
entries. The physical Vision Pro captured `AOT_GAME_HOOK_OK`,
`FLAMINGO_GAME_HOOK_OK`, `SCENE_HOOK_ARMED` at libunity +0x5f2830 and
`LATE_PHASE_OK`. Unity's original call and Flamingo self-uninstall precede
Scotland2's late dispatch. The user confirmed normal menu access. Game data
was retained. The console later records a normal background exit with code 0.
Evidence: `port-results/headset-hooks-20261023/`. Actual early/late mod folders
remain empty. The subsequent actual custom-types source port is described below.

## Actual custom-types source port

`SC2_CT_REGISTER=1 bash ports/scotland2/test-customtypes.sh` passes on this Mac.
Original custom-types 0.18.3 now imports adapted And64 instead of compiling the
embedded engine. A small Unity lookup patch binds its original signature to an
exact raw-ELF profile because offline TLS translation changes the signature.
The signed runtime requires a matching prepared entry. Source/patch/license and
provenance are in `customtypes/`; the original source input remains untouched.

The actual AutoRegister implementation registers both built-in coroutine
classes once. Game class-name and class-type APIs resolve them, and actual
IL2CPP object allocation creates instances with those classes. This is actual
registration behavior, not just constructor/import resolution. Signed code
inspection uses the real Capstone through a data view of original entry words
and PCs. Evidence: `customtypes-registration.log`, `capstone-original-view.log`.
The physical Vision Pro test also passes, with normal menu access confirmed
by the user; evidence is in `port-results/headset-customtypes-20261023/`. This historical custom-types milestone predates the physical Lapiz/BSML
results above. SongCore callback verification remains pending.

Heavy Paper logging exposed a runtime futex wake-count bug. Real waiter counts,
address/mask matching and conditional WAKE_OP now pass focused tests; the
previously stalled actual SDK initialization completes. The existing Paper
flush-wait API bug remains distinct. Android allocator XML dumping is explicitly
unsupported (`malloc_info` returns failure), while its GC checks stay enabled.
`SC2_TEST_SET=customtypes bash ports/scotland2/build-test-app.sh` builds/signs
the same test ID for registration verification during the original scene phase.

### October 24: original song browser update installed, physical result pending

Final actual-source host composition passed after final probes/guards:
MetaCore564/PlaylistCore1495/SongDownloader1845 imports, zero missing; all real
setup/type/hook tests and shared hook chains; actual browser parsed20 BeatSaver
maps using standalone Mozilla trust asset. Evidence browser-final-source-host.log.
Original browser rejected local untrusted certificate with curl60. Vision Pro
native runtime, all-load link gate, Swift typecheck, full signed app build and
strict deep signature verification passed. Prepared378 game entries (126 targets
at depth2), three Unity targets (scene/custom-types/measured deletion).

Installed successfully on physical paired Vision Pro via devicectl into the
same dev.klepton.beatsaber140.scotland2.test, display Beat Saber (Scotland2 Test).
Receipt port-results/headset-songdownloader-20261024/install.json; application
container AF960016-6606-41C4-85FF-26AA6AE763A8. No new app ID or slot; ordinary
app untouched. Metadata contains actual Lapiz/BSML/MetaCore early modules and
actual PlaylistCore/SongCore/SongDownloader late modules; original networking
support and public standalone CA asset included. New physical startup probes
verify MetaCore immediate/deferred real Unity destruction and scheduler Update,
actual SongDownloader menu registration and async original BeatSaver API query.

Asked user to unlock/open existing test app and select SongDownloader in Mods,
then report results/error/closure. That answer is currently pending. Do not claim
physical MetaCore/PlaylistCore/SongDownloader success, download or playback from
the host results or successful install. If the app runs, get the finished boot
log after user closes it (active growing logs often interrupt device transfer).
Use bounded120-second copy; never unpair/reboot the headset or overwrite game
data. User has no songs and expects in-game download; do not ask for manual
song folders again.
