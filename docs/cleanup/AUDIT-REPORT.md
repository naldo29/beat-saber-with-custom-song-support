# Change audit

Source review date: 2026-10-25. The introduced/adapted source review is complete;
final release-package and physical-candidate gates remain in progress. This is
not an approval of checks that have not run.

The review covers the integration delta from Klepton base
`0333512150a91a9ac83ceeae6e89c2d9ddc44eb3`, the cleanup of working port
`e89e51fb24a63c4534a9b5a3ef10f4a2d7c55a42`, original-source identity and active
loader, hook, filesystem, network, save and UI boundaries. CHANGE-INVENTORY.json
records file classifications, hashes, mapped review notes and evidence scope.
Unchanged imports are identified and reconstructed, not represented as a new
line-by-line security review of every upstream implementation.

## Findings and resolution

| Severity | Trigger and impact | Resolution | Performed verification |
| --- | --- | --- | --- |
| High | Remote ZIP entries can escape their destination, create links, overwrite a song or expand without bounds. | Validate every entry; bound sizes/count/depth; reject traversal, links, special files and collisions; stream through the pinned original parser into a private stage. Publish only a complete new destination. Original SongDownloader uses this boundary; the released BeatSaverPlusPlus keeps its full binary/API with only ExtractAll routed through a prepared signed hook. | Native ASan/UBSan adversarial fixtures and translated calls through both original extraction entries passed, including preservation of existing content. |
| High | SongDetails disables TLS verification and supplies a C++ string to curl's default header FILE callback. | Rebuild original SongDetails with its original protobuf 3.21.12/Brotli 1.1.0, verified peer/host and explicit CA, HTTPS-only redirects and a bounded real header callback. | All 84 required public namespace exports preserved; actual translated verified HTTPS and untrusted-certificate rejection passed. |
| High | WebUtils supplies a four-byte int to curl's long HTTP-status output. | Use long; bound body/header callbacks, handle failed initialization and free request headers. | Current source compilation and actual translated networking/constructor/import tests passed. |
| High | SongDetails trusts remote packed hashes, counts and tag shifts; short hashes read beyond their input. | Validate hash/song/tag/difficulty counts before access or publication, bound Brotli output, initialize immutable hash fields safely, retain async inputs and preserve prior state on rejected replacement. | Actual original-code fixtures passed valid processing, malformed hashes/counts/tags/difficulties, previous-state preservation and invalid Brotli rejection. |
| High | Backup replacement can truncate the last good copy or restore stale data over a newer live save. | Exclusive temporary creation, bounded complete copy, fsync/close and same-directory rename; contain failures and serialize game save plus backup. Restore only an eligible backup. | Native ASan/UBSan preservation/collision/link fixtures and real translated IL2CPP WriteAllText/File.Replace/normal/local recovery checks passed in isolated storage. |
| Medium | Native mkstemp cannot translate Android aliases within its caller-sized buffer. | Reserve backup temporary files through guest-aware exclusive open; never clean up a name not successfully created by this operation. | The translated save fixture caught this after native tests passed. Initial attempts failed; the corrected translated fixture passed. No live player data was used. |
| Medium | Download completion/progress can mutate Unity UI from workers, access a reused row or race error text; playlist completion can free shared state too soon. | Main-thread UI/playlist dispatch, retained Unity owners and atomic search generations, synchronized internal error snapshots, shared completion state, exact/empty queue handling and request-before-move ordering. | Original SongDownloader compiled and passed current translated API/type/setup/network regressions. Interaction and lifecycle behavior still require the final headset test. |
| Medium | Invalid URLs can throw before a request; progress exceptions can unwind through curl's C callback. | Reject malformed URL construction, clamp finite progress and turn callback exceptions into request failure. | Actual translated WebUtils and SongDownloader callback/URL fixtures passed. |
| Medium | A canceled futex waiter can leave a stack node/mutex live; time arithmetic can overflow; requested wake counts create phantom writers. | Track exact address/mask waiters, remove them before cancellation resumes, check overflow/invalid arguments, and report actual selected wakes. | Focused timeout, cancellation, zero/exact wake, mask/address and conditional wake-operation regressions passed. |
| Medium | Missing unused DT_NEEDED entries or unresolved imports can reach constructors. | Strict, scoped module opens reject missing/excessive dependencies, cycles/depth overflow and missing symbols before initialization. | Trapping-constructor missing-import and missing-unused-dependency fixtures passed. |
| Medium | Weak C++ metadata can initialize independently in separate mod DSOs. | Canonicalize only default-visible weak/unique objects during scoped strict mod loads; preserve hidden/protected storage and local functions. | Actual guest-DSO shared-object/once-initialization and isolation regressions passed. |
| Medium | Unicode can grow atlases indefinitely or cache a blank glyph after failed rasterization. | Bound the original cache to 32 atlases, validate scalar/root/allocation state and rasterize successfully before cache insertion; preserve original TMP fallback/style/atlas and retained region upload. | Native style/whitespace/scalar/CJK/emoji checks and translated glyph/upload-API checks passed. Actual final Unity rendering remains a device gate. |
| Medium | Progressive selections, stale outputs, a missing nested dependency pin and source-tree link writes undermine clean builds. | One selected manifest and authoritative builder, diagnostic-only guards on nine old helpers, fresh release output, tracked Capstone pin and generated BSML include overlay. | Source reconstruction, original-input coherence, adversarial bootstrap tests and an earlier extracted source-only signed build passed; current final builds are recorded separately. |
| Medium | Temporary UI, saved test overrides and GC tracing remain enabled despite a release label. | Remove observer/test exports and Unicode test UI, disable the Advanced/Test panel and its overrides, omit three custom-types GC tracing hooks and the domain-get probe. Retain the required scene adapter. | Release core compilation and Swift typecheck passed; the validator rejects the earlier GC-tracing candidate. Final actual app flags/components passed structural validation after both isolated builds; the Swift test-controls gate is present in the final candidate. |
| Medium | The start window reads an entire prior boot log into SwiftUI; a 19 MB log can stall text layout during scene creation. | Read at most a 16 KiB tail, decode partial UTF-8 safely and avoid publishing identical previews. Preserve the complete file. | A physical termination report showed a scene-create watchdog in main-thread text layout. Native regression passed short/missing/empty logs, original-file preservation, a 1 GiB sparse file and invalid UTF-8. Final app rebuild/signature checks recorded externally; no further manual test requested. |
| Medium | Generated target-table drift removes existing games. | Deterministic table generation with retained pinned-base generic rows. | Two-environment generation/legacy-row checks and mod-disabled native/Swift/dlopen regressions passed. |

All identified high/critical findings in the introduced/adapted source scope have
been addressed. User-confirmed downloads/playback, save/backup readback and normal exit are
recorded alongside the package. A later console-attached restart exposed the
bounded-log-preview finding above. The final preview change is verified
automatically; further manual testing was declined by the user. Final archive
reconciliation and exact evidence remain separately recorded. Component/hook repeatability and actual unsigned recipient configuration
passed within the scope in VALIDATION-REPORT.md. This statement is not
a comprehensive audit of all unchanged third-party internals or every dependency
version and does not establish compatibility with arbitrary inputs or mods.

## Architecture and lifecycle limits

One versioned optional C callback table keeps mod policy out of the generic
loader. The port owns its host bridge and CoreText renderer; the original native
Scotland2 loader remains separate, and genuine Android C++ objects remain in
translated guest modules. Exact prepared entries are validated before signing;
runtime hook changes only publish pointers in writable data.

Guest modules have process lifetime. dlclose does not unmap them. General hot
unloading, unknown interleaved hook engines, midpoint hooks, caller-provided
executable trampolines and arbitrary QMOD injection are unsupported. The selected
original startup order and shared backend are the supported composition.

TLS is required for networking. The original local-file WebUtils API remains a
local operation. Bounds reject unsupported large downloads/archives/databases;
no unlimited resource, all-map or extended-session guarantee is asserted.

## Provenance and publication

Fifteen source trees containing 1,031 recorded files were independently matched
to immutable archives and reconstructed with their adaptation patches. Twenty-one
original dependency locks, 29 public archive/static dependency pins and 15
selected QMOD inputs were checked. The selected ten core mods exclude Fake ID.

Some exact upstream archives contain no license file. The source-only builder
fetches original trees locally; it does not redistribute those trees or original
QMOD binaries. Available original notices, source locations, official public CA
license and unresolved license metadata are retained in THIRD-PARTY-NOTICES.md.
No blanket redistribution clearance is asserted.

The actual source archive is checked against its manifest and scanned for paid
binaries, provisioning/signing material, private paths/device identifiers, links
and obsolete/excluded components. Raw logs, game inputs, signed apps, maps and
saves remain private. Maintainer review groups are prepared locally; upstream
submission, acceptance and merging are not implied.
