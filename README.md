# Beat Saber with Custom Song Support

Standalone custom songs for the Quest version of Beat Saber on Apple Vision Pro,
using Klepton and the actual Scotland2 mod stack. Forked from
[myMartek/Klepton](https://github.com/myMartek/Klepton), built for AVP Play.

## What works

Tested on a physical Vision Pro with Beat Saber **1.40.8_7379**:

- Scotland2's real dependency loading and early/late mod startup.
- Original SongDownloader in-game browsing, downloading and extraction.
- Original SongCore loading and playback of downloaded custom songs.
- Repeated downloads: SongCore loaded one, then two, then three songs in the tested session.
- custom-types, Lapiz, BSML, Paperlog, MetaCore and PlaylistCore running together.

This uses the original mods. The separate earlier song-loader prototype is disabled
(`KL_SONGS=0`) in the Scotland2 build.

## Installation status

**Developer source preview. There is no ready-to-install public IPA or one-click
AVP Play integration yet.** The successful build was assembled and tested locally.
A fresh-machine dependency/bootstrap process still needs packaging and validation.
See [CUSTOM-SONGS.md](CUSTOM-SONGS.md) for the build layout and limitations, and
[BUILDING.md](BUILDING.md) for the underlying Klepton prerequisites.

Bring your own purchased Beat Saber game files. This repository contains no APK,
OBB, translated Beat Saber frameworks, saved games, songs, or Apple signing keys.
The prepared hooks are tied to the exact tested game build; other versions must
be independently prepared and verified.

The result installed on a headset is one visionOS app with the adapted mods
baked into its signed bundle. An Android APK patched with mods alone cannot
replace the required modified Klepton runtime.

## What is still unverified

- Every map format, every playlist feature, full-song completion and long sessions.
- Restart/update persistence across all settings and scores.
- Arbitrary third-party Quest mods. This is not a universal QMOD installer.
- The original custom-song baseline has eight core mods enabled. The remaining
  Unicode, PlayerDataKeeper and Fake ID ports now build and pass host checks;
  their all-eleven-mod device update is built/signed, but physical tests are pending.

## Source and credits

Klepton is by Max Thomas, with the AVP Play toolchain work by myMartek and its
contributors. Original upstream documentation is retained in
[UPSTREAM-README.md](UPSTREAM-README.md). Original mod sources, licenses, version
pins and source patches are under [ports/scotland2](ports/scotland2). Dependency
pins are under `port-deps/sources`; the public QMOD inventory is under
`port-results/coremods/coremods-lock.json`. Generated builds, device logs and
private signing material are excluded. Retain the relevant upstream licenses
when redistributing components.
