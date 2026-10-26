# Supported composition

The builder supports the exact game identified by the library and metadata
SHA-256 values in `ports/scotland2/selected-mods.json`. Matching a version label
alone is insufficient. Other versions, arbitrary QMODs, stock AVP Play integration
and App Store delivery are outside this validation.

| Component | Scope |
| --- | --- |
| Game | Beat Saber 1.40.8_7379; exact original library and metadata hashes required |
| Host build | Apple-silicon Mac; Xcode 27.1, visionOS SDK 27.0 used for current host checks |
| Guest C++ toolchain | Original Android NDK r27c, including a verified extracted file tree |
| Graphics | Declared ANGLE/MoltenVK cache, individually hash-verified; clean graphics-source rebuild not claimed |
| Hook backend | Prepared signed entries and writable dispatch slots; JIT remains disabled |
| App updates | Reuse the installed app identifier and signing team; never uninstall/reset its data |
| Device validation | See VALIDATION-REPORT.md; historical observations and final-candidate checks are distinct |

| Original package | Version | Role |
| --- | --- | --- |
| custom-types | 0.18.3 | Selected core mod |
| lapiz | 0.2.23 | Selected core mod |
| bsml | 0.4.55 | Selected core mod |
| songcore | 1.1.24 | Selected core mod |
| playlistcore | 1.5.5 | Selected core mod |
| paper2_scotland2 | 4.6.4 | Selected core mod |
| songdownloader | 0.8.8 | Selected core mod |
| unicode | 0.1.7 | Selected core mod |
| datakeeper | 0.2.5 | Selected core mod |
| metacore | 1.6.2 | Selected core mod |
| beatsaber-hook | 6.4.2 | Supporting dependency |
| tinyxml2 | 10.0.0 | Supporting dependency |
| web-utils | 0.6.8 | Supporting dependency |
| beatsaverplusplus | 0.2.1 | Supporting dependency |
| song-details | 1.0.4 | Supporting dependency |

These 15 original QMOD packages supply 16 original ELF libraries. The release
composition contains 19 metadata ELF files after adding the required ABI and
scene support modules, plus the native loader and guest C++ runtime frameworks.
Exact component paths are authoritative in the selection manifest. Fake ID and
observer DSOs are excluded.

Runtime modules have process lifetime. General hot unloading, midpoint hooks,
caller-supplied executable trampolines, injected plugins, unlimited download
sizes, and all-map/long-session compatibility are not established. The local
clean-output build uses independently verified original inputs and is not a
second-Mac or recipient-environment validation.
