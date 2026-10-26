# Beat Saber custom songs for visionOS

A standalone Klepton builder for Beat Saber **1.40.8_7379**, using the original
Scotland2 loader, SongCore and SongDownloader. It prepares selected native hooks
before code signing and keeps executable-memory restrictions enabled.

The selected composition has ten original core mods: custom-types, Lapiz, BSML,
SongCore, PlaylistCore, Paper, SongDownloader, Unicode, PlayerDataKeeper and
MetaCore. Fake ID is excluded. Exact versions, inputs and runtime components are
recorded in [selected-mods.json](ports/scotland2/selected-mods.json).

Release mode removes the temporary Unicode test button, observer modules and
port test controls, including the Advanced › Test panel and its saved overrides. Original mod menus and song downloading remain part of the
application. PlayerDataKeeper backs up saves through atomic replacement; updates
must retain the same bundle identifier and application data.

## Build and update

See [CUSTOM-SONGS.md](CUSTOM-SONGS.md) for the supported inputs and builder
command. Supply your own matching game, Apple signing team and app identifier.
Build outputs are private and separate from this source tree. The builder does
not install, launch, uninstall or reset application data.

The shareable source package contains first-party code, adaptation patches,
verified download manifests and notices. It reconstructs original third-party
source locally from pinned public inputs. Game files, maps, saves, signed apps,
provisioning profiles and private evidence are excluded.

## Verification and scope

[Audit report](docs/cleanup/AUDIT-REPORT.md) ·
[Validation report](docs/cleanup/VALIDATION-REPORT.md) ·
[Release checklist](docs/cleanup/RELEASE-CHECKLIST.md) ·
[Architecture](docs/cleanup/ARCHITECTURE.md)

Those reports distinguish completed checks from pending checks. A successful
build does not establish device gameplay, long-session stability, arbitrary QMOD
compatibility, other game versions or stock AVP Play integration. This is a
standalone builder, not an App Store package.

[Klepton's original documentation](UPSTREAM-README.md) and [license](LICENSE)
remain available. Original sources and their licenses remain attributed in the
input manifests and third-party notices.
