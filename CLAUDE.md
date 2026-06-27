# CLAUDE.md

Guidance for Claude Code (and other AI assistants) working in this repository.

## What this is

**CloudSeedReverb** — an algorithmic reverb audio plugin built with the [JUCE](https://juce.com) framework. It is a GUI port of Valdemar Erlingsson's Cloud Seed, based on xunil-cloud's JUCE adaptation of the algorithm.

* Plugin formats: **AU**, **VST3**, **Standalone** (set in `CMakeLists.txt` via `FORMATS`)
* Platforms: **macOS** (Universal: arm64 + x86_64), **Windows** x64, **Linux** x64
* Company: `Pinkcloud Engineering` · Plugin code `Dem0` · Manufacturer code `Juce`
* Project version: see `project(CloudSeedReverb VERSION ...)` in `CMakeLists.txt`

## Layout

| Path | Purpose |
| --- | --- |
| `CMakeLists.txt` | Top-level build config (`juce_add_plugin`, formats, deployment target) |
| `audio_engine/` | The Cloud Seed reverb DSP |
| `plugin/` | JUCE processor + editor (UI) |
| `JUCE/` | JUCE framework (git submodule) |
| `exports/` | Screenshot and legacy prebuilt bundles |
| `release/` | Packaged release zips for the GitHub Releases page |
| `.github/workflows/build.yml` | CI: builds + validates on macOS/Windows/Linux, publishes release on `v*` tags |

`build/` is git-ignored.

## Build

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release --parallel
```

Artifacts: `build/CloudSeedReverb_artefacts/Release/{AU,VST3,Standalone}/`.
macOS output is a Universal binary — verify with `lipo -info <binary>`.

## Releasing — IMPORTANT packaging rule

The macOS release zips must contain the plugin bundle **at the archive root** (e.g. `CloudSeedReverb.component/...`), with **no `../` path components**.

A previous release shipped zips made with a command like `zip -r out.zip ../build/.../CloudSeedReverb.component` from inside `release/`. That embedded `../build/...` paths into every entry. macOS Archive Utility / The Unarchiver refuse to extract entries that escape the archive root (Zip Slip protection), so users saw an **empty archive** and DAWs couldn't find the plugin. Command-line `unzip` masked the problem because it tolerates the traversal.

**Always package macOS bundles with `ditto` from inside the artifacts dir:**

```bash
cd build/CloudSeedReverb_artefacts/Release
ditto -c -k --sequesterRsrc --keepParent AU/CloudSeedReverb.component   CloudSeed-AU-macOS-universal.zip
ditto -c -k --sequesterRsrc --keepParent VST3/CloudSeedReverb.vst3      CloudSeed-VST3-macOS-universal.zip
```

Verify before publishing — this must print `0`:

```bash
unzip -l <zip> | grep -c '\.\./'
```

The CI `release` job (in `build.yml`) packages from the downloaded artifact dirs and does **not** have this problem; the traversal bug only affected manually-built zips.

## macOS distribution caveat

Builds are **not code-signed or notarized**, so downloaded bundles are quarantined by Gatekeeper. Users may need:

```bash
xattr -dr com.apple.quarantine <bundle>
```

This is documented in the README and is the usual reason a freshly downloaded plugin doesn't show up in a DAW.
