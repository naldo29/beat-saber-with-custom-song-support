# Integration review

This is a standalone Klepton builder for the exact supported Beat Saber version.
The AVP Play desktop application is unchanged; no stock AVP Play compatibility or
maintainer acceptance is claimed.

The cleanup uses one optional built-in guest-extension callback table instead of
putting Scotland2 policy in the generic loader. The port owns its bridge and the
Apple Unicode renderer. Original Scotland2 remains a separate signed framework;
original mod code remains in Android modules. General compatibility fixes and
prepared signed dispatch are separately identifiable in the change ledger.

This preserves the tested native ABI with a small optional boundary. Moving every
callback into another dynamic framework would require a larger host ABI for JNI,
image lookup, paths and hook operations. That alternative was not chosen merely
to reduce line counts. See [the architecture decision](docs/cleanup/ARCHITECTURE.md).

Release mode removes the unused song-probe wiring, observer packages, test UI and
progressive-composition release selection. The original Flamingo scene adapter
remains because it performs required startup work. A single manifest drives the
release metadata, embedding and hook catalogs. Fake ID stays excluded.

The builder takes explicit owner inputs and recipient signing settings, uses
separate outputs, checks supported game hashes before compilation and validates
the actual app composition. Source-only packaging reconstructs original sources
from verified archives and patches; private game/signing material is excluded.

This is a reviewed integration boundary, not a proof that every possible future
Klepton integration must have the same shape. Full audit findings, performed
checks and pending physical/fresh-environment checks are recorded in the reports.
