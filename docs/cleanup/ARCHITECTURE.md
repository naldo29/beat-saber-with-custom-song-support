# Integration boundary

The host exposes one optional built-in guest extension with a versioned C callback
table: startup, explicit symbol and library ownership, a symbol interposer, and
scoped strict module loading. It is registered before guest threads start and has
process lifetime. This is deliberately a single built-in extension, not a general
plugin loader. Normal builds link no Scotland2 bridge or CoreText renderer.

Scotland2 policy lives in `ports/scotland2/src/host-bridge.c`, compiled only with
`KLEPTON_SCOTLAND2=1`. The original native Scotland2 loader remains its own signed
Scotland2Core framework; original Android C++ objects stay in translated mod DSOs.
The Apple Unicode renderer is a leaf of the port. The prepared signed-entry
backend stays in the host, coupled only to translated-image attach/detach.

Moving every callback into a separately loaded framework would require a much
larger host ABI for image lookup, JNI, filesystem state, hook registration and
error reporting. A statically linked optional leaf preserves the tested native
ABI while removing policy from the generic loader. It does not claim the host has
zero modifications or that AVP Play accepts this integration.

The source groups for review are: generic compatibility (futex, errno, OBB guard,
allocator/zlib support), generic extension and prepared hooks, optional Scotland2
bridge and Unicode leaf, original mod adaptations, and deterministic builder.
Release builds retain only the Flamingo scene adapter needed for original
lifecycle timing; type, networking, save and Unity observer fixtures are diagnostic.
