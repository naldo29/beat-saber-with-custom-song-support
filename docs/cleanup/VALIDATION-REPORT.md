# Validation record

Updated 2026-10-25. Source/host/candidate checks below are complete within their
stated scope. Final archive checksum/build reconciliation and headset acceptance
are recorded alongside the delivered archive. Historical headset observations
are not current-candidate results.

| Check | Result and scope |
| --- | --- |
| Recorded Git baseline/tree and clean starting checkout | Matched the handoff; work retained on a separate cleanup branch. |
| Original game libraries, metadata, dependency locks, private evidence and tested rollback | Recorded hashes matched; all 166 rollback files matched; deep strict signature verification passed using normal Mac signing services. Rollback retained. |
| Original ten-mod baseline rebuilt | Passed after diagnosing missing clean-checkout input/directory prerequisites. |
| Native visionOS and Swift, Scotland2 disabled | Passed on Xcode 27.1 / visionOS SDK 27.0; ordinary dlopen delegation/symbol/unsupported-flag regression passed with JIT disabled. Simulator runtime not tested. |
| Original loader phases/ownership/dependency parsing | Four phase/failure scenarios, 16 actual library dependency inventories and 11 malformed metadata fixtures passed. |
| Prepared ARM64 hooks/native gates | Integer/stack/FP/structure-return/PC-relative ABI, chain/restore, concurrent dispatch and unchanged-code checks passed; 25 wrong-input/metadata/instruction/output-alias cases rejected. |
| Strict missing imports/dependencies and weak data | Trapping constructors were not executed; missing imports/unused dependencies rejected. Weak default-visible objects shared once; hidden/protected storage isolated. |
| Original SongCore collector and OBB behavior | Actual collector scanned isolated nested songs repeatedly on main/worker threads; Unity empty-Unreal-base guard and positive Unreal virtual-directory regression passed. |
| Futex and errno | Exact/zero wakes, address/mask isolation, conditional operations, timeout removal, cancellation and overflow checks passed. |
| ZIP boundaries | Native ASan/UBSan adversarial/preservation fixtures and static analyzer passed. Actual translated original BeatSaverPlusPlus entry and original SongDownloader extraction passed valid, duplicate-preservation and traversal checks. |
| TLS and callbacks | Actual translated WebUtils, SongDetails and SongDownloader verified HTTPS and rejected the isolated untrusted certificate. Invalid-URL and progress-callback failure fixtures passed. Original BeatSaver API fetched/parsed a nonempty page. |
| SongDetails data boundaries | Actual original-code hash/count/tag/difficulty/corrupted-Brotli fixtures passed, including a valid database and preservation of prior state on malformed replacement. Required public exports preserved. |
| Save backup/recovery | Native ASan/UBSan failure/link/collision/newer-live-data fixtures passed. Translated fixture first exposed native temporary-name alias failures; corrected guest-aware exclusive creation then passed original IL2CPP normal/local WriteAllText/File.Replace/restore paths. No live save corruption or reset. |
| Unicode | Native CJK/color-emoji/style/whitespace/invalid-scalar tests and translated raster/upload-API tests passed. Final Unity texture/cache behavior remains a device check. |
| Original source identity and patches | 15 trees / 1,031 files matched and reconstructed after final source adaptations. |
| Dependency/bootstrap coherence | 21 baseline locks, 29 public archive/static pins and 15 selected QMODs checked. Current safe extractor reproduced all 8,625 NDK files/links/modes; hostile-archive extraction fixtures passed. |
| Release diagnostics | Native release loader and Swift source checks passed; nine progressive helpers reject release mode. Actual final app passed GC/UI/export/selection validation, including all three release-control flags set to zero; no debug dylib is packaged. |
| Extracted source-only empty-output builds | Two isolated empty-output builds completed and passed deep strict signature verification. The second includes the Swift test-controls gate; the first is comparison evidence. The later bounded-preview fix receives a targeted regression and signed rebuild, recorded beside the final archive. Earlier failed/intermediate candidates are retained separately. |
| Signed composition before bounded-preview fix and repeated component/hook manifests | Passed: 144 app files, 19 metadata ELF files and 21 mod/support frameworks. All 31 metadata/profile/certificate/provenance files and both game/Unity hook reports are byte-identical between isolated builds. 129 original game targets / 387 prepared entries and three Unity entries; four removed targets were diagnostic consumers. Signed app bundles are not claimed byte-identical. |
| Recipient bundle-ID generation/packaging without provisioning/install | Passed actual alternate-ID project generation, unsigned Release app build and structural validation using final prepared frameworks. Existing project identity restored. No additional app ID allocated or installed; separate full bootstrap/--unsigned-builder execution not claimed. |
| Final source archive extraction/build/inspection/checksum mapping | Artifact-dependent result and checksum are recorded beside the archive; inner documents do not self-identify their containing archive. |
| Maintainer review groups | Ten review patches, five each against pinned Klepton base and recorded working baseline, passed ordered application in isolated temporary Git indexes. Shared dependencies and omitted historical private comment context are documented. |
| Physical menu/download/playback/save/normal exit | Same-ID installation passed after 152-file backup. 53 song/save/settings files were byte-identical before first launch. User confirmed two new Michael Jackson songs worked flawlessly; six song folders observed, a new valid score persisted, live save matched DataKeeper backup and all 25 original song files remained unchanged. Full 321-second run log ended by normal background exit, without native crash markers. |
| Restart and visual checks | User broadly confirmed the restart/score/Japanese/full-song/emoji prompt and declined further manual testing. A separate console-attached restart was killed by the scene-create watchdog in text layout; the unbounded log preview was then capped. No claim of a new physical retest of that final fix, independent emoji capture, extended-session stability or all-map coverage. |
| Bounded start-window log preview | Native regression passed short/missing/empty, long-file preservation, 1 GiB sparse seek and invalid-UTF-8 bounds. Preview reads at most 16 KiB and preserves the complete diagnostic file. Final app build/signature and archive mapping are recorded externally. |
| Second Mac/recipient environment and graphics source rebuild | Not performed. |

The source-only builds use independently verified public input caches, including
an extracted original NDK and a declared graphics cache. They do not borrow
prepared mod/game frameworks from the populated development workspace. An
isolated build on this Mac is distinct from a fresh Mac or recipient validation.

Native sanitizers instrument isolated adapters, not translated Android execution.
Host checks do not prove actual Unity interaction, rendering, score persistence,
all maps or long sessions. The earlier physical baseline established its recorded
menu/custom-song/downloading/Unicode/save scope only.

Raw logs, intermediate failures, build/source/input receipts and device evidence
are retained privately. The final release record maps the exact source and
package checksum to performed checks; private team/device/game/save material is
excluded from the shareable source archive.
