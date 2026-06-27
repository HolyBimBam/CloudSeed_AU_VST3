# Cloud Seed — AudioUnit & VST3 Reverb

![screenshot](https://github.com/HolyBimBam/CloudSeed_AU_VST3/blob/master/exports/screenshot.png)

A cross-platform port of the **Cloud Seed** algorithmic reverb, rebuilt with the [JUCE](https://juce.com) framework. Builds as **AudioUnit (AU)**, **VST3**, and **Standalone** for **macOS** (Universal — Apple Silicon + Intel), **Windows**, and **Linux**.


## Table of contents
* [General info](#general-info)
* [Download & Install](#download--install)
* [Documentation](#documentation)
* [How to Build](#how-to-build)
* [To Do](#to-do)
* [Credits](#credits)


## General Info

CloudSeed is an artificial reverberation plugin based on the reverb algorithm written by Valdemar Erlingsson, who also implemented the original VST version for Windows: [CloudSeed VST](https://github.com/ValdemarOrn/CloudSeed).

While studying the algorithm and trying to get it running on different Apple machines, I came across [xunil-cloud's changes](https://github.com/xunil-cloud/CloudReverb) that adapt it to the JUCE framework. Since that was a bare implementation without a graphical user interface, I recreated Valdemar's Windows UI in JUCE.


## Download & Install

Download the latest build from the [Releases page](https://github.com/HolyBimBam/CloudSeed_AU_VST3/releases). Each release contains:

| File | Platform | Formats |
| --- | --- | --- |
| `CloudSeed-AU-macOS-universal.zip` | macOS (Apple Silicon + Intel) | AudioUnit |
| `CloudSeed-VST3-macOS-universal.zip` | macOS (Apple Silicon + Intel) | VST3 |
| `CloudSeed-VST3-Windows.zip` | Windows x64 | VST3 |
| `CloudSeed-VST3-Linux.zip` | Linux x64 | VST3 |

### macOS

Unzip the archive, then copy the bundle into your plugin folder:

* **AudioUnit** → `~/Library/Audio/Plug-Ins/Components/CloudSeedReverb.component`
* **VST3** → `~/Library/Audio/Plug-Ins/VST3/CloudSeedReverb.vst3`

> **Apple Silicon / Gatekeeper note:** the binaries are **not code-signed or notarized**, so macOS quarantines them after download. If your DAW (Ableton, Logic, etc.) doesn't see the plugin, clear the quarantine flag in Terminal:
>
> ```bash
> xattr -dr com.apple.quarantine ~/Library/Audio/Plug-Ins/Components/CloudSeedReverb.component
> xattr -dr com.apple.quarantine ~/Library/Audio/Plug-Ins/VST3/CloudSeedReverb.vst3
> ```
>
> Then rescan plugins in your DAW. In Logic, run the AudioUnit validation again.

### Windows

Unzip and copy `CloudSeedReverb.vst3` into `C:\Program Files\Common Files\VST3\`, then rescan plugins in your DAW.

### Linux

Unzip and copy `CloudSeedReverb.vst3` into `~/.vst3/` (or `/usr/lib/vst3/` system-wide), then rescan plugins in your DAW.


## Documentation
For the algorithm and parameter documentation, head over to the [original Documentation page](https://github.com/ValdemarOrn/CloudSeed/tree/master/Documentation) for an overview of the user interface and an explanation of the reverberation kernel.


## How to Build

The project builds with **CMake** and bundles JUCE as a git submodule. macOS builds produce a Universal binary (arm64 + x86_64). The same flow runs in CI for macOS, Windows, and Linux — see [`.github/workflows/build.yml`](.github/workflows/build.yml).

#### 1. Clone the repository (with submodules)

```bash
git clone --recursive https://github.com/HolyBimBam/CloudSeed_AU_VST3.git
cd CloudSeed_AU_VST3
```

If you already cloned without `--recursive`:

```bash
git submodule update --init --recursive
```

#### 2. Configure

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
```

#### 3. Build

```bash
cmake --build build --config Release --parallel
```

The built plugins land in:

```
build/CloudSeedReverb_artefacts/Release/AU/CloudSeedReverb.component
build/CloudSeedReverb_artefacts/Release/VST3/CloudSeedReverb.vst3
build/CloudSeedReverb_artefacts/Release/Standalone/
```

### Linux build dependencies

```bash
sudo apt-get install -y libasound2-dev libjack-jackd2-dev ladspa-sdk \
  libcurl4-openssl-dev libfreetype6-dev libx11-dev libxcomposite-dev \
  libxcursor-dev libxext-dev libxinerama-dev libxrandr-dev libxrender-dev \
  libwebkit2gtk-4.0-dev libglu1-mesa-dev mesa-common-dev
```


## To Do

* Make it work reliably with Logic
* Show correct scale for nonlinear parameters
* Fix loading error when a preset is already selected
* Add icons to dials or a graphical representation of what each parameter does
* Code-sign and notarize the macOS builds
* Buy a JUCE licence and remove the splash popup


## Credits

* [CloudSeed VST](https://github.com/ValdemarOrn/CloudSeed) by Valdemar Erlingsson
* [CloudReverb JUCE Implementation](https://github.com/xunil-cloud/CloudReverb) by xunil-cloud
