# CloudSeed Roadmap

Open improvements and suggestions for future development.

---

## Performance

### SIMD Optimization for DSP Processing
**Priority:** Medium

The Cloud Seed algorithm is CPU-intensive due to high-order delay lines and feedback loops. SIMD optimization could significantly reduce CPU load.

**Suggested Implementation:**
- Use `juce::dsp::SIMDRegister` for vectorized processing
- Consider Intel IPP or Apple Accelerate frameworks
- Profile the code first to identify CPU bottlenecks (ReverbChannel, DelayLine, AllpassDiffuser)

**References:**
- [JUCE SIMD Documentation](https://docs.juce.com/master/classdsp_1_1SIMDRegister.html)
- [Apple Accelerate Framework](https://developer.apple.com/documentation/accelerate)

---

## UI/UX

### Add Logarithmic Parameter Scaling
**Priority:** High

Many parameters (frequency, time) should use logarithmic scaling so the "sweet spot" isn't crammed into one end of the knob's rotation.

**Suggested Implementation:**
- Use `juce::NormalisableRange` with skew factor in `createParameterLayout()`
- Example: `NormalisableRange<float>(20.0f, 20000.0f, 0.0f, 0.3f)` for frequency

**Affected Parameters:**
- PostLowShelfFrequency
- PostHighShelfFrequency
- PostCutoffFrequency
- Predelay
- All delay/time parameters

---

### High-DPI / Retina Support
**Priority:** Medium

On macOS Retina displays, graphics may appear blurry if using low-resolution assets.

**Suggested Implementation:**
- Replace PNG assets with vector-based graphics using `juce::Path` or SVGs
- Ensure all custom drawing uses `Graphics::setImageResamplingQuality()`

---

### Resizable GUI
**Priority:** Low

Modern producers often work on 4K monitors.

**Suggested Implementation:**
- Add `juce::ResizableCornerComponent`
- Or implement a scale setting (75%, 100%, 150%, 200%)
- Use relative positioning instead of fixed pixel coordinates

---

### Visual Feedback
**Priority:** Low

Add real-time visualization of the reverb tail or a spectrum analyzer to show what the "Damping" and "Filter" sections are doing.

---

## Features

### Implement Factory Presets
**Priority:** Medium

The preset methods are commented out in PluginProcessor.cpp. Re-enable and properly integrate them.

**Tasks:**
- Uncomment `initFactoryDullEchos()`, `initFactoryHyperplane()`, etc.
- Implement `loadPresetProgram()`
- Sync preset values to treeState
- Add meaningful program names in `getProgramName()`

---

### Custom Preset Browser
**Priority:** Low

Rather than relying on the DAW's generic preset menu, implement a custom JUCE-based preset browser that supports `.preset` files.

---

## Licensing

### Remove JUCE Evaluation Popup
**Priority:** High (for distribution)

Requires purchasing a JUCE license or using the personal/education tier correctly.

**Options:**
- Purchase JUCE Indie ($40/month) or Pro license
- Verify compliance with JUCE personal license terms
- Alternative: Port to iPlug2 (MIT licensed)

---

## Completed

- [x] Universal Binary support (ARM64 + x86_64)
- [x] JUCE 8.0.9 upgrade
- [x] Parameter sync fix (treeState synchronization)
- [x] Sample rate handling
- [x] Tail length reporting
- [x] GitHub Actions CI/CD
- [x] pluginval validation (strictness 5)
- [x] auval validation
