# Klepton / AVP Play integration review

Reviewed October 24, 2026, against the transferred Klepton base
`0333512150a91a9ac83ceeae6e89c2d9ddc44eb3` and this working source port.
This is a source review and device-tested implementation, not proof of the
smallest possible patch or compatibility with every newer AVP Play release.

## Assessment

The approach preserves the actual Scotland2 loader and original custom-song
mods. It fits Klepton's existing offline translation and signed-code model.
The current fork is a working developer integration; it still has test-specific
code and has not been reduced to a minimal upstream patch.

The AVP Play desktop application's code was not changed. The changes are in the
Klepton runtime, the game build process and the resulting standalone game app.
Klepton is the compatibility layer AVP Play uses to build these applications.
See the [AVP Play toolchain source](https://github.com/myMartek/Klepton/tree/avp-play).

## Why dropping the original mods into an APK is insufficient

Quest mods load Android shared libraries and install hooks by changing executable
code at runtime. The Klepton build we started with provides neither this port's
Scotland2 startup integration nor its prepared hook dispatch. The released hook
engines also contain private/inlined implementations; replacing one exported
hook function does not replace those copies.

On visionOS, executable code must be signed, including embedded libraries;
ordinary apps cannot assume arbitrary unsigned or self-modifying native code.
[Apple's code-signing description](https://support.apple.com/guide/security/app-code-signing-process-sec7c917bf14/web)
explains this platform constraint. Here, the translator prepares exact game hook
entries before signing, then runtime hook installation changes writable pointers.
No new executable-memory permission is introduced (`KL_GUEST_JIT=0`).

This requires build/runtime support somewhere. It does not prove that every
current edit must live in Klepton's core, or that a permanent fork is necessary.
More of the bridge could be packaged as a separate signed framework with a thin
host interface; the toolchain would still need to embed, initialize and connect
that framework to guest loading and symbols. That alternative has not been built
or tested in this project.

## Current responsibilities and how to minimize them

| Area | Current implementation | Cleaner integration boundary |
| --- | --- | --- |
| Mod loading and startup timing | Scotland2Core framework plus `kl_scotland2.c`, guest loader/symbol seams and IL2CPP/scene lifecycle interception | Keep Scotland2 ordering and game-specific policy in its framework; expose a small optional host interface for startup, guest loading and symbol resolution. |
| Hooking without executable writes | Offline `prepare_hooks.py`, signed game entries, `kl_aot_hooks.c`, image attach/detach | Keep the manifest game-specific and the signed dispatch backend reusable. Image callbacks could reduce direct loader coupling; that refactor needs validation. |
| Android compatibility fixes | Scoped weak-object sharing, strict mod import checks, correct futex wake counts and the Unity/OBB directory guard | Maintain as individually reviewable compatibility patches with their existing regression checks. They affect shared infrastructure and require broader validation before claiming no other-game impact. |
| Unicode drawing | Original TMP cache/atlas plus Apple CoreText C bridge and retained Unity texture upload | Keep the platform renderer as a leaf of the Unicode support package; it need not permanently become a generic Klepton feature. |
| Packaging | Translated/signed mod frameworks, original dependency metadata, exact game hook manifests and certificate trust | Add an optional mod-pack build selection to the AVP Play toolchain. Keep existing game versions and normal builds independent. |
| Test code | Multiple lifecycle probes and staged test flags; older song probe remains present but disabled | Separate required adapters from observations and fixtures before producing a release build. Do not blindly remove files named “probe”: the current scene-hook adapter uses one of them for actual startup. |

## Remaining cleanup before calling this minimal

- Separate diagnostics and host fixtures from required runtime adapters. The old
  `kl_songprobe` experiment is not used for custom-song loading and need not ship
  in a release. The stock upstream song implementation is disabled in this build;
  original SongCore/SongDownloader supply the functionality.
- Replace the progressive test-set packaging with one reproducible selected-mod
  manifest, plus a release build path. The legacy `build-test-app.sh` validator
  currently recognizes fewer compositions than the package scripts and is not a
  validated one-command release installer.
- Exclude unrelated generated target-table differences from an upstream patch.
  The table differs from the pinned base while its `visionos/targets.py` generator
  is unchanged; those differences are not a requirement for Beat Saber modding.
- Separate general compatibility fixes, generic hook support and game-specific
  mod integration into reviewable patches. A disabled Scotland2 path alone is
  not evidence that every shared compatibility change leaves all other games
  unaffected.
- Validate a clean-machine build and recipient signing flow. The current local
  generator still uses the existing test app ID; no generic installer is claimed.

Most of the fork's large source addition is vendored original mods/dependencies,
not rewritten game logic. Before this review's Fake ID removal, the runtime,
visionOS frontend and Makefile comparison touched eleven existing files
(279 added / 48 removed lines) and added eight files (1,081 lines). These counts
exclude port sources and tests and include development scaffolding. They describe
the current surface area, not a guarantee that every added line is necessary.

## Fake ID removal

Fake ID is redundant for this setup because Klepton already supplies the adult
age-category response. The selected package now includes ten original core mods.
Fake ID is absent from packaged libraries, embedded frameworks, prepared hook
selection and active host/device checks. The original inventory and unused port
source remain as historical references.

The Mac checks, device runtime build, app signature and same-ID installation
passed. The user confirmed the menu and a custom song still work. The headset log
shows PlayerDataKeeper and Unicode's eight hooks, real Unity texture-region
write/readback, original CJK/emoji glyph caching and four custom songs loaded;
there is no Fake ID module load. This update preserves the existing app data and
uses the same free-account app slot.
