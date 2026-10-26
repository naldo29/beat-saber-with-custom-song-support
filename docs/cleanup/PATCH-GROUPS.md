# Maintainer review groups

The review artifacts separate the complete integration against Klepton base
`0333512150a91a9ac83ceeae6e89c2d9ddc44eb3` from the cleanup of the recorded working
port at `e89e51fb24a63c4534a9b5a3ef10f4a2d7c55a42`.

| Group | Contents and purpose |
| --- | --- |
| Generic compatibility | Futex waiter counts/cancellation/timeout/errno and the Unity-versus-Unreal directory guard; focused regressions. |
| Prepared hooks and optional interface | Immutable prepared code, writable pointer dispatch, strict module loading and symbol ownership, weak C++ data canonicalization, one versioned optional C callback table. |
| Shared build/frontend wiring | Optional source selection and frameworks, deterministic retained target table, recipient identity and release controls. Includes allocator/zlib shim additions where they share interface-wiring files. |
| Scotland2 component and mod adaptations | Native original loader boundary, guest ABI/scene adapters, glyph renderer, bounded extraction/atomic saves, original-mod patches and exact source identities. |
| Builder and documentation | Explicit input bootstrap, one selected composition, source-only packaging, notices, audit/validation records and instructions. |

The groups are review units with shared wiring and ordered dependencies, not a
promise that every patch applies independently. No unchanged third-party source
tree or paid/signing artifact belongs in these patches. Original adaptations are
represented by the per-project upstream.patch files and pinned reconstruction
manifests. Historical Fake ID source is excluded from review-package outputs.

The patch index records each group's paths and SHA-256. Maintainer submission,
acceptance or merging is external work and has not been performed by producing
these local artifacts.

Both ordered sets passed application in isolated temporary Git indexes against
their recorded bases. Original line endings/license text were retained, so
whitespace warnings in nested upstream patches were not normalized. The diff of
build_run_viewer.sh is omitted because its sole cleanup change removes an old
author-local path from a comment; the current file is in the source archive.
The index names this omission. No active code change is omitted for that reason.
