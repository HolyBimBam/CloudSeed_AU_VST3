# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

CloudSeed is an artificial reverberation audio plugin for macOS, providing AudioUnit (AU) and VST3 formats using the JUCE framework. Based on Valdemar Erlingsson's reverberation algorithm.

## Build Commands

```bash
# Clone with JUCE submodule
git clone --recursive https://github.com/xunil-cloud/CloudSeed_AU_VST3.git

# Build
cd CloudSeedAUVST3
mkdir build && cd build
cmake ../ -B .
make
```

Requires CMake 3.15+. Outputs: `CloudSeed.component` (AU), `CloudSeed.vst3`, and standalone app.

## Architecture

### Audio Engine (`audio_engine/`)

Namespace: `CloudSeed::`

**Signal Flow:**
```
Input → PreDelay → MultitapDiffuser → AllpassDiffuser → DelayLines[12] → Biquads → Output
```

Key classes:
- **ReverbController** - Main orchestrator, manages dual stereo channels with 2048-sample buffers
- **ReverbChannel** - Per-channel processing with 12 parallel delay lines
- **DelayLine** - Combines modulated delay + allpass diffuser + biquad filters
- **ModulatedDelay** - Delay with LFO modulation
- **AllpassDiffuser** - Up to 8 stages of modulated allpass filters
- **MultitapDiffuser** - Early reflections with decay envelope

Audio utilities in `AudioLib/` namespace: Biquad filters (7 types), first-order HP/LP, SHA-256 RNG for seed generation.

### Plugin Wrapper (`plugin/`)

- **PluginProcessor** - JUCE AudioProcessor with stereo I/O bus, manages 46 parameters via AudioProcessorValueTreeState
- **PluginEditor** - 1300x500 pixel UI with 40+ rotary knobs, preset selector

### Parameter System

46 parameters defined in `Parameter.h` enum, organized into categories: Input, Filtering, Early Reflections, Diffusion, Late Delay Lines, Modulation, Frequency Response, Seeds, and Output Levels.

## Code Patterns

- Header-only DSP components (most of `audio_engine/` is .h files)
- Parameter enum in `Parameter.h` drives all DSP updates
- JUCE ValueTreeState for parameter persistence and UI binding
- 2048-sample internal processing buffers, 2-second delay buffers

## Known Issues (from README)

- Logic Pro compatibility issues
- Incorrect scale display for nonlinear parameters
- Preset loading error when already selected
- JUCE evaluation popup (requires commercial license)
