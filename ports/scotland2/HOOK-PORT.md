> Current status: original SongDownloader downloads and SongCore custom-song playback
> were confirmed on physical Vision Pro on October 24, 2026. See [CUSTOM-SONGS.md](../../CUSTOM-SONGS.md).
> The notes below retain development history and may describe earlier incomplete stages.

# Signed hook port: implemented scope

The Mac now executes hooks through source adapters for the actual And64 public
API and Flamingo 1.1.2 installer. Selected game entry points are prepared before
signing. Runtime installation changes writable dispatch pointers; executable
memory restrictions remain enabled (`KL_GUEST_JIT=0`). This is still a partial
Scotland2/dependency port. No actual custom-song mod has run.

## Implementation

`tools/prepare_hooks.py` binds an explicit target manifest to the exact raw ELF
and translated Mach-O hashes and original instruction words. It adds signed
entry gates and original-call trampolines, plus writable pointer slots. Existing
guest addresses are preserved. Unsupported instructions and wrong builds fail.
Runtime `kl_aot_hooks.c` validates signed metadata, segment bounds and gate words,
then publishes original pointers before hooks become reachable. It does not
recompute the raw ELF hash; the signed image protects the offline result.

The And64 adapter preserves `A64HookFunction` original-call/chaining behavior.
Caller-supplied executable buffers (`A64HookFunctionV`) fail explicitly.
Flamingo retains its original registry, size-only metadata validation,
default/final priorities, reinstall and uninstall. It preserves a preceding
And64 hook; changes are serialized across the adapters. It refuses to overwrite
an intervening engine's later hook. Midpoint hooks are unsupported.

Released beatsaber-hook and some mods embed hidden/inlined And64 engines.
Exporting the adapted public API does not replace those copies. Source clients
must be rebuilt or their callsites individually adapted and verified. The old
released beatsaber-hook constructor still requests executable memory and is
refused; loading that library is not evidence that its embedded hooks work.

## Mac evidence

- `aot-reload-verified.log`: signed ARM64 execution, stack/integer/FP/structure
  results, PC-relative relocation, chaining, concurrent pointer updates,
  unchanged executable bytes, unload/reset and retained-image reload. Twenty
  wrong-build/instruction/malformed signed-record cases are rejected.
- `scene-fixture-verified.log`: separate Android DSOs exercise both actual
  source adapters, original Flamingo priority/metadata/chain behavior and
  refusal of conflicting engine mutation. The scene helper calls the original,
  self-uninstalls and invokes the callback exactly once.
- `both-engines-game-final.log`: actual Beat Saber IL2CPP initialization with
  both hook adapters; both hook milestones and 145 assemblies pass.

Logs are under `port-results/session-20261023/`.

## Unity late lifecycle

`tools/find_scene_target.py` follows Scotland2's original lookup from
`UnityEngine.Object::DestroyImmediate`: first RET, preceding last BL, first B.
For this exact game the final target is libunity + `0x5f2830`. The generated
profile records the ELF hash and intermediate addresses. Runtime independently
uses the actual IL2CPP icall resolver and repeats the lookup; a mismatch with a
prepared signed entry fails closed. The guest helper uses actual Flamingo to
call the original, remove its hook, then dispatch the native Scotland2 late
phase once. The signed fixture and the physical Vision Pro test pass. The headset logs
`AOT_GAME_HOOK_OK`, `FLAMINGO_GAME_HOOK_OK`, `SCENE_HOOK_ARMED` and
`LATE_PHASE_OK`; the user confirmed the menu works. Evidence is under
`port-results/headset-hooks-20261023/`.

`SC2_TEST_SET=hooks bash ports/scotland2/build-test-app.sh` builds/signs the
same existing test app with these adapters and two prepared game entries. It
never installs by itself. Use only `dev.klepton.beatsaber140.scotland2.test`;
the user's free account has a three-app limit.

## Remaining

The bundle has no actual early/late mods. Actual custom-types registration/name/type lookup and allocation now pass on
both the Mac and physical headset (see README). Continue rebuilding hidden
engine consumers, resolving complete Android C++ registry objects/game file
inspection, and advancing BSML, Lapiz and SongCore through actual callbacks.
The virtual libil2cpp basename does not yet satisfy file-size/HookTracker uses.
BL-entry/unwind-aware trampolines, midpoint hooks, arbitrary unprepared targets
and runtime-generated executable code are unsupported. Custom-song playback
has not been implemented or verified; no separate song loader was added.
