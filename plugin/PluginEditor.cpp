#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    juce::ignoreUnused (processorRef);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    
    potSize = 80;
    
    
    firstRow = 150;
    secondRow = 260;
    
    firstCol = 15;
    secondCol = firstCol + potSize*4.25;    // 355;
    thirdCol = secondCol + potSize*4.3125; //700;
    fourthCol = thirdCol + potSize*4.3125; //1045;


    
    firstRowFont = firstRow + potSize*0.9375;
    secondRowFont = secondRow + potSize*0.9375;
    thirdRowFont = secondRowFont + potSize*0.4;
    
    setSize (1300, 500);

//    auto& paramsEditor = processor.getParameters();
//    juce::AudioParameterFloat* inputMixParameter = ((juce::AudioParameterFloat*)paramsEditor.getUnchecked(0));
    
    inputMixSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.treeState, "inputMix", inputMixSlider );
    inputMixSlider.setSliderStyle (juce::Slider::RotaryVerticalDrag);
    inputMixSlider.setRange (0.0f, 1.0f, 0.0001f);
    inputMixSlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 120, 20);
    inputMixSlider.setPopupDisplayEnabled (true, false, this);
//    inputMixSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::orangered);
    inputMixSlider.addListener (this);
    addAndMakeVisible (&inputMixSlider);
    
    predelaySliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.treeState, "predelay", predelaySlider );
    predelaySlider.setSliderStyle (juce::Slider::RotaryVerticalDrag);
    predelaySlider.setRange (0.0f, 1000.0f, 0.1f);
    predelaySlider.setTextValueSuffix(" ms");
    predelaySlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 120, 20);
    predelaySlider.setPopupDisplayEnabled (true, false, this);
    predelaySlider.addListener (this);
    addAndMakeVisible (&predelaySlider);
    
    lowCutSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.treeState, "lowCut", lowCutSlider );
    lowCutSlider.setSliderStyle (juce::Slider::RotaryVerticalDrag);
    lowCutSlider.setRange (0.0f, 1.0f, 0.0001f);
    lowCutSlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 120, 20);
    lowCutSlider.setPopupDisplayEnabled (true, false, this);
    lowCutSlider.addListener (this);
    addAndMakeVisible (&lowCutSlider);
    
    highCutSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.treeState, "highCut", highCutSlider );
    highCutSlider.setSliderStyle (juce::Slider::RotaryVerticalDrag);
    highCutSlider.setRange (0.0f, 1.0f, 0.0001f);
    highCutSlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 120, 20);
    highCutSlider.setPopupDisplayEnabled (true, false, this);
    highCutSlider.addListener (this);
    addAndMakeVisible (&highCutSlider);

    
    
    dryOutSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.treeState, "dryOut", dryOutSlider );
    dryOutSlider.setSliderStyle (juce::Slider::RotaryVerticalDrag);
    dryOutSlider.setRange (0.0f, 1.0f, 0.0001f);
    dryOutSlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 120, 20);
    dryOutSlider.setPopupDisplayEnabled (true, false, this);
    dryOutSlider.addListener (this);
    addAndMakeVisible (&dryOutSlider);
    
    predelayOutSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.treeState, "predelayOut", predelayOutSlider );
    predelayOutSlider.setSliderStyle (juce::Slider::RotaryVerticalDrag);
    predelayOutSlider.setRange (0.0f, 1.0f, 0.0001f);
    predelayOutSlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 120, 20);
    predelayOutSlider.setPopupDisplayEnabled (true, false, this);
    predelayOutSlider.addListener (this);
    addAndMakeVisible (&predelayOutSlider);
    
    earlyOutSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.treeState, "earlyOut", earlyOutSlider );
    earlyOutSlider.setSliderStyle (juce::Slider::RotaryVerticalDrag);
    earlyOutSlider.setRange (0.0f, 1.0f, 0.0001f);
    earlyOutSlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 120, 20);
    earlyOutSlider.setPopupDisplayEnabled (true, false, this);
    earlyOutSlider.addListener (this);
    addAndMakeVisible (&earlyOutSlider);
    
    mainOutSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.treeState, "mainOut", mainOutSlider );
    mainOutSlider.setSliderStyle (juce::Slider::RotaryVerticalDrag);
    mainOutSlider.setRange (0.0f, 1.0f, 0.0001f);
    mainOutSlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 120, 20);
    mainOutSlider.setPopupDisplayEnabled (true, false, this);
    mainOutSlider.addListener (this);
    addAndMakeVisible (&mainOutSlider);
    
    
    
    
    tapCountSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.treeState, "tapCount", tapCountSlider );
    tapCountSlider.setSliderStyle (juce::Slider::RotaryVerticalDrag);
    tapCountSlider.setRange (0.0f, 1.0f, 0.0001f);
    tapCountSlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 120, 20);
    tapCountSlider.setPopupDisplayEnabled (true, false, this);
    tapCountSlider.addListener (this);
    addAndMakeVisible (&tapCountSlider);
    
    tapLengthSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.treeState, "tapLength", tapLengthSlider );
    tapLengthSlider.setSliderStyle (juce::Slider::RotaryVerticalDrag);
    tapLengthSlider.setRange (0.0f, 500.0f, 0.0001f);
    tapLengthSlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 120, 20);
    tapLengthSlider.setPopupDisplayEnabled (true, false, this);
    tapLengthSlider.setTextValueSuffix(" ms");
    tapLengthSlider.addListener (this);
    addAndMakeVisible (&tapLengthSlider);
    
    diffusionDelaySliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.treeState, "diffusionDelay", diffusionDelaySlider );
    diffusionDelaySlider.setSliderStyle (juce::Slider::RotaryVerticalDrag);
    diffusionDelaySlider.setRange (0.0f, 1.0f, 0.0001f);
    diffusionDelaySlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 120, 20);
    diffusionDelaySlider.setPopupDisplayEnabled (true, false, this);
    diffusionDelaySlider.addListener (this);
    addAndMakeVisible (&diffusionDelaySlider);
    
    earlyDiffusionModAmountSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.treeState, "earlyDiffusionModAmount", earlyDiffusionModAmountSlider );
    earlyDiffusionModAmountSlider.setSliderStyle (juce::Slider::RotaryVerticalDrag);
    earlyDiffusionModAmountSlider.setRange (0.0f, 2.5f, 0.0001f);
    earlyDiffusionModAmountSlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 120, 20);
    earlyDiffusionModAmountSlider.setPopupDisplayEnabled (true, false, this);
    earlyDiffusionModAmountSlider.addListener (this);
    addAndMakeVisible (&earlyDiffusionModAmountSlider);
    
    
    

    
    tapGainSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.treeState, "tapGain", tapGainSlider );
    tapGainSlider.setSliderStyle (juce::Slider::RotaryVerticalDrag);
    tapGainSlider.setRange (0.0f, 1.0f, 0.0001f);
    tapGainSlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 120, 20);
    tapGainSlider.setPopupDisplayEnabled (true, false, this);
    tapGainSlider.addListener (this);
    addAndMakeVisible (&tapGainSlider);
    
    tapDecaySliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.treeState, "tapDecay", tapDecaySlider );
    tapDecaySlider.setSliderStyle (juce::Slider::RotaryVerticalDrag);
    tapDecaySlider.setRange (0.0f, 1.0f, 0.0001f);
    tapDecaySlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 120, 20);
    tapDecaySlider.setPopupDisplayEnabled (true, false, this);
    tapDecaySlider.addListener (this);
    addAndMakeVisible (&tapDecaySlider);
    
    diffusionFeedbackSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.treeState, "diffusionFeedback", diffusionFeedbackSlider );
    diffusionFeedbackSlider.setSliderStyle (juce::Slider::RotaryVerticalDrag);
    diffusionFeedbackSlider.setRange (0.0f, 1.0f, 0.0001f);
    diffusionFeedbackSlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 120, 20);
    diffusionFeedbackSlider.setPopupDisplayEnabled (true, false, this);
    diffusionFeedbackSlider.addListener (this);
    addAndMakeVisible (&diffusionFeedbackSlider);
    
    earlyDiffusionModRateSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.treeState, "earlyDiffusionModRate", earlyDiffusionModRateSlider );
    earlyDiffusionModRateSlider.setSliderStyle (juce::Slider::RotaryVerticalDrag);
    earlyDiffusionModRateSlider.setRange (0.0f, 1.0f, 0.0001f);
    earlyDiffusionModRateSlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 120, 20);
    earlyDiffusionModRateSlider.setPopupDisplayEnabled (true, false, this);
    earlyDiffusionModRateSlider.addListener (this);
    addAndMakeVisible (&earlyDiffusionModRateSlider);
    
    
    
    lineDelaySliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.treeState, "lineDelay", lineDelaySlider );
    lineDelaySlider.setSliderStyle (juce::Slider::RotaryVerticalDrag);
    lineDelaySlider.setRange (0.0f, 1.0f, 0.0001f);
    lineDelaySlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 120, 20);
    lineDelaySlider.setPopupDisplayEnabled (true, false, this);
    lineDelaySlider.addListener (this);
    addAndMakeVisible (&lineDelaySlider);
    
    lineModAmountSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.treeState, "lineModAmount", lineModAmountSlider );
    lineModAmountSlider.setSliderStyle (juce::Slider::RotaryVerticalDrag);
    lineModAmountSlider.setRange (0.0f, 2.5f, 0.0001f);
    lineModAmountSlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 120, 20);
    lineModAmountSlider.setPopupDisplayEnabled (true, false, this);
    lineModAmountSlider.addListener (this);
    addAndMakeVisible (&lineModAmountSlider);
    
    lateDiffusionDelaySliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.treeState, "lateDiffusionDelay", lateDiffusionDelaySlider );
    lateDiffusionDelaySlider.setSliderStyle (juce::Slider::RotaryVerticalDrag);
    lateDiffusionDelaySlider.setRange (0.0f, 1.0f, 0.0001f);
    lateDiffusionDelaySlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 120, 20);
    lateDiffusionDelaySlider.setPopupDisplayEnabled (true, false, this);
    lateDiffusionDelaySlider.addListener (this);
    addAndMakeVisible (&lateDiffusionDelaySlider);
    
    lateDiffusionModAmountSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.treeState, "lateDiffusionModAmount", lateDiffusionModAmountSlider );
    lateDiffusionModAmountSlider.setSliderStyle (juce::Slider::RotaryVerticalDrag);
    lateDiffusionModAmountSlider.setRange (0.0f, 2.5f, 0.0001f);
    lateDiffusionModAmountSlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 120, 20);
    lateDiffusionModAmountSlider.setPopupDisplayEnabled (true, false, this);
    lateDiffusionModAmountSlider.addListener (this);
    addAndMakeVisible (&lateDiffusionModAmountSlider);
    

    lineDecaySliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.treeState, "lineDecay", lineDecaySlider );
    lineDecaySlider.setSliderStyle (juce::Slider::RotaryVerticalDrag);
    lineDecaySlider.setRange (0.0f, 1.0f, 0.0001f);
    lineDecaySlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 120, 20);
    lineDecaySlider.setPopupDisplayEnabled (true, false, this);
    lineDecaySlider.addListener (this);
    addAndMakeVisible (&lineDecaySlider);
    
    lineModRateSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.treeState, "lineModRate", lineModRateSlider );
    lineModRateSlider.setSliderStyle (juce::Slider::RotaryVerticalDrag);
    lineModRateSlider.setRange (0.0f, 1.0f, 0.0001f);
    lineModRateSlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 120, 20);
    lineModRateSlider.setPopupDisplayEnabled (true, false, this);
    lineModRateSlider.addListener (this);
    addAndMakeVisible (&lineModRateSlider);
    
    lateDiffusionFeedbackSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.treeState, "lateDiffusionFeedback", lateDiffusionFeedbackSlider );
    lateDiffusionFeedbackSlider.setSliderStyle (juce::Slider::RotaryVerticalDrag);
    lateDiffusionFeedbackSlider.setRange (0.0f, 1.0f, 0.0001f);
    lateDiffusionFeedbackSlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 120, 20);
    lateDiffusionFeedbackSlider.setPopupDisplayEnabled (true, false, this);
    lateDiffusionFeedbackSlider.addListener (this);
    addAndMakeVisible (&lateDiffusionFeedbackSlider);
    
    lateDiffusionModRateSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.treeState, "lateDiffusionModRate", lateDiffusionModRateSlider );
    lateDiffusionModRateSlider.setSliderStyle (juce::Slider::RotaryVerticalDrag);
    lateDiffusionModRateSlider.setRange (0.0f, 1.0f, 0.0001f);
    lateDiffusionModRateSlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 120, 20);
    lateDiffusionModRateSlider.setPopupDisplayEnabled (true, false, this);
    lateDiffusionModRateSlider.addListener (this);
    addAndMakeVisible (&lateDiffusionModRateSlider);
    
  
    
    postLowShelfFrequencySliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.treeState, "postLowShelfFrequency", postLowShelfFrequencySlider );
    postLowShelfFrequencySlider.setSliderStyle (juce::Slider::RotaryVerticalDrag);
    postLowShelfFrequencySlider.setRange (0.0f, 1.0f, 0.0001f);
    postLowShelfFrequencySlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 120, 20);
    postLowShelfFrequencySlider.setPopupDisplayEnabled (true, false, this);
    postLowShelfFrequencySlider.addListener (this);
    addAndMakeVisible (&postLowShelfFrequencySlider);
    
    postHighShelfFrequencySliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.treeState, "postHighShelfFrequency", postHighShelfFrequencySlider );
    postHighShelfFrequencySlider.setSliderStyle (juce::Slider::RotaryVerticalDrag);
    postHighShelfFrequencySlider.setRange (0.0f, 1.0f, 0.0001f);
    postHighShelfFrequencySlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 120, 20);
    postHighShelfFrequencySlider.setPopupDisplayEnabled (true, false, this);
    postHighShelfFrequencySlider.addListener (this);
    addAndMakeVisible (&postHighShelfFrequencySlider);
    
    postCutoffFrequencySliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.treeState, "postCutoffFrequency", postCutoffFrequencySlider );
    postCutoffFrequencySlider.setSliderStyle (juce::Slider::RotaryVerticalDrag);
    postCutoffFrequencySlider.setRange (0.0f, 1.0f, 0.0001f);
    postCutoffFrequencySlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 120, 20);
    postCutoffFrequencySlider.setPopupDisplayEnabled (true, false, this);
    postCutoffFrequencySlider.addListener (this);
    addAndMakeVisible (&postCutoffFrequencySlider);
    
    
    postLowShelfGainSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.treeState, "postLowShelfGain", postLowShelfGainSlider );
    postLowShelfGainSlider.setSliderStyle (juce::Slider::RotaryVerticalDrag);
    postLowShelfGainSlider.setRange (0.0f, 1.0f, 0.0001f);
    postLowShelfGainSlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 120, 20);
    postLowShelfGainSlider.setPopupDisplayEnabled (true, false, this);
    postLowShelfGainSlider.addListener (this);
    addAndMakeVisible (&postLowShelfGainSlider);
    
    postHighShelfGainSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.treeState, "postHighShelfGain", postHighShelfGainSlider );
    postHighShelfGainSlider.setSliderStyle (juce::Slider::RotaryVerticalDrag);
    postHighShelfGainSlider.setRange (0.0f, 1.0f, 0.0001f);
    postHighShelfGainSlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 120, 20);
    postHighShelfGainSlider.setPopupDisplayEnabled (true, false, this);
    postHighShelfGainSlider.addListener (this);
    addAndMakeVisible (&postHighShelfGainSlider);
    
    crossSeedSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.treeState, "crossSeed", crossSeedSlider );
    crossSeedSlider.setSliderStyle (juce::Slider::RotaryVerticalDrag);
    crossSeedSlider.setRange (0.0f, 1.0f, 0.0001f);
    crossSeedSlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 120, 20);
    crossSeedSlider.setPopupDisplayEnabled (true, false, this);
    crossSeedSlider.addListener (this);
    addAndMakeVisible (&crossSeedSlider);
    
    
    
    
    diffusionStagesSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.treeState, "diffusionStages", diffusionStagesSlider );
    diffusionStagesSlider.setSliderStyle (juce::Slider::LinearBar);
    diffusionStagesSlider.setNormalisableRange(juce::NormalisableRange<double> (1, 8, 1.0));
    diffusionStagesSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 120, 20);
//    diffusionStagesSlider.setValue(4);
    diffusionStagesSlider.addListener (this);
    addAndMakeVisible (&diffusionStagesSlider);
    
    
    lineCountSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.treeState, "lineCount", lineCountSlider );
    lineCountSlider.setSliderStyle (juce::Slider::LinearBar);
    lineCountSlider.setNormalisableRange(juce::NormalisableRange<double> (1, 12, 1.0));
    lineCountSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 120, 20);
//    lineCountSlider.setValue(12);
    lineCountSlider.addListener (this);
    addAndMakeVisible (&lineCountSlider);
    

    
    lateDiffusionStagesSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.treeState, "lateDiffusionStages", lateDiffusionStagesSlider );
    lateDiffusionStagesSlider.setSliderStyle (juce::Slider::LinearBar);
    lateDiffusionStagesSlider.setNormalisableRange(juce::NormalisableRange<double> (1, 8, 1.0));
    lateDiffusionStagesSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 120, 20);
//    lateDiffusionStagesSlider.setValue(3);
    lateDiffusionStagesSlider.addListener (this);
    addAndMakeVisible (&lateDiffusionStagesSlider);
    
    
    
    
    
    tapSeedSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.treeState, "tapSeed", tapSeedSlider );
    tapSeedSlider.setSliderStyle (juce::Slider::LinearBar);
//    tapSeedSlider.setRange (0.0f, 1.0f, 0.0001f);
    tapSeedSlider.setRange (0, 1000000, 1);

    tapSeedSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 120, 20);
    tapSeedSlider.addListener (this);
    addAndMakeVisible (&tapSeedSlider);
    
    diffusionSeedSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.treeState, "diffusionSeed", diffusionSeedSlider );
    diffusionSeedSlider.setSliderStyle (juce::Slider::LinearBar);
    diffusionSeedSlider.setRange (0, 1000000, 1);
    diffusionSeedSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 120, 20);
    diffusionSeedSlider.addListener (this);
    addAndMakeVisible (&diffusionSeedSlider);
    
    delaySeedSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.treeState, "delaySeed", delaySeedSlider );
    delaySeedSlider.setSliderStyle (juce::Slider::LinearBar);
    delaySeedSlider.setRange (0, 1000000, 1);
    delaySeedSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 120, 20);
    delaySeedSlider.addListener (this);
    addAndMakeVisible (&delaySeedSlider);
    
    postDiffusionSeedSliderAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.treeState, "postDiffusionSeed", postDiffusionSeedSlider );
    postDiffusionSeedSlider.setSliderStyle (juce::Slider::LinearBar);
    postDiffusionSeedSlider.setRange (0, 1000000, 1);
    postDiffusionSeedSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 120, 20);
    postDiffusionSeedSlider.addListener (this);
    addAndMakeVisible (&postDiffusionSeedSlider);
    
    
//    diffusionEnabledButton.onClick = [&]() { processorRef.diffusionEnabledButtonValue = processorRef.diffusionEnabledButtonValue < 0.5f ? 1.0f : 0.0f;  };
    diffusionEnabledButtonAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(processorRef.treeState, "diffusionEnabled", diffusionEnabledButton );
    diffusionEnabledButton.onClick = [&]() { };
//    diffusionEnabledButton.setToggleState(true, juce::NotificationType::dontSendNotification);
    diffusionEnabledButton.setColour(juce::TextButton::ColourIds::buttonColourId , juce::Colours::grey);
    diffusionEnabledButton.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::darkorange);
    diffusionEnabledButton.addListener (this);
    addAndMakeVisible(diffusionEnabledButton);

//    lateDiffusionEnabledButton.onClick = [&]() { processorRef.lateDiffusionEnabledButtonValue = processorRef.lateDiffusionEnabledButtonValue < 0.5f ? 1.0f : 0.0f;  };
//    lateDiffusionEnabledButton.onClick = [&]() { toggleLateDiffusion();  };
    lateDiffusionEnabledButtonAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(processorRef.treeState, "lateDiffusionEnabled", lateDiffusionEnabledButton );
    lateDiffusionEnabledButton.onClick = [&]() {  };
//    lateDiffusionEnabledButton.setToggleState(true, juce::NotificationType::dontSendNotification);
    lateDiffusionEnabledButton.setColour(juce::TextButton::ColourIds::buttonColourId , juce::Colours::grey);
    lateDiffusionEnabledButton.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::darkorange);
    lateDiffusionEnabledButton.addListener (this);
    addAndMakeVisible(lateDiffusionEnabledButton);

    hiPassEnabledButtonAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(processorRef.treeState, "highPassEnabled", hiPassEnabledButton );
    hiPassEnabledButton.onClick = [&]() {  };
//    hiPassEnabledButton.setToggleState(false, juce::NotificationType::dontSendNotification);
    hiPassEnabledButton.setColour(juce::TextButton::ColourIds::buttonColourId , juce::Colours::darkgrey);
    hiPassEnabledButton.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::darkorange);
    hiPassEnabledButton.addListener (this);
    addAndMakeVisible(hiPassEnabledButton);

    lowPassEnabledButtonAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(processorRef.treeState, "lowPassEnabled", lowPassEnabledButton );
    lowPassEnabledButton.onClick = [&]() {  };
//    lowPassEnabledButton.setToggleState(false, juce::NotificationType::dontSendNotification);
    lowPassEnabledButton.setColour(juce::TextButton::ColourIds::buttonColourId , juce::Colours::darkgrey);
    lowPassEnabledButton.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::darkorange);
    lowPassEnabledButton.addListener (this);
    addAndMakeVisible(lowPassEnabledButton);
    
    
    lowShelfEnabledButtonAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(processorRef.treeState, "lowShelfEnabled", lowShelfEnabledButton );
    lowShelfEnabledButton.onClick = [&]() {  };
//    lowShelfEnabledButton.setToggleState(false, juce::NotificationType::dontSendNotification);
    lowShelfEnabledButton.setColour(juce::TextButton::ColourIds::buttonColourId , juce::Colours::grey);
    lowShelfEnabledButton.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::darkorange);
    lowShelfEnabledButton.addListener (this);
    addAndMakeVisible(lowShelfEnabledButton);
    
    highShelfEnabledButtonAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(processorRef.treeState, "highShelfEnabled", highShelfEnabledButton );
    highShelfEnabledButton.onClick = [&]() {  };
//    highShelfEnabledButton.setToggleState(false, juce::NotificationType::dontSendNotification);
    highShelfEnabledButton.setColour(juce::TextButton::ColourIds::buttonColourId , juce::Colours::grey);
    highShelfEnabledButton.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::darkorange);
    highShelfEnabledButton.addListener (this);
    addAndMakeVisible(highShelfEnabledButton);
    
    cutoffEnabledButtonAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(processorRef.treeState, "cutoffEnabled", cutoffEnabledButton );
    cutoffEnabledButton.onClick = [&]() {  };
//    cutoffEnabledButton.setToggleState(true, juce::NotificationType::dontSendNotification);
    cutoffEnabledButton.setColour(juce::TextButton::ColourIds::buttonColourId , juce::Colours::grey);
    cutoffEnabledButton.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::darkorange);
    cutoffEnabledButton.addListener (this);
    addAndMakeVisible(cutoffEnabledButton);
    
    lateStageTapButtonAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(processorRef.treeState, "lateStageTap", lateStageTapButton );
    lateStageTapButton.onClick = [&]() {  };
//    lateStageTapButton.setToggleState(true, juce::NotificationType::dontSendNotification);
    lateStageTapButton.setColour(juce::TextButton::ColourIds::buttonColourId , juce::Colours::grey);
    lateStageTapButton.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::darkorange);
    lateStageTapButton.addListener (this);
    addAndMakeVisible(lateStageTapButton);
    
    interpolationButtonAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(processorRef.treeState, "interpolation", interpolationButton );
    interpolationButton.onClick = [&]() {  };
//    interpolationButton.setToggleState(false, juce::NotificationType::dontSendNotification);
    interpolationButton.setColour(juce::TextButton::ColourIds::buttonColourId , juce::Colours::grey);
    interpolationButton.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::darkorange);
    interpolationButton.addListener (this);
    addAndMakeVisible(interpolationButton);
    
    
    
    addAndMakeVisible(presetSelector);

    presetSelector.addItem("Chorus Delay", presetSelector.getNumItems() + 1);
    presetSelector.addItem("Dull Echoes", presetSelector.getNumItems() + 1);
    presetSelector.addItem("Hyperplane", presetSelector.getNumItems() + 1);
    presetSelector.addItem("Medium Space", presetSelector.getNumItems() + 1);
    presetSelector.addItem("Noise in the Hallway", presetSelector.getNumItems() + 1);
    presetSelector.addItem("Rubi-Ka Fields", presetSelector.getNumItems() + 1);
    presetSelector.addItem("Small Room", presetSelector.getNumItems() + 1);
    presetSelector.addItem("The 90s Are Back", presetSelector.getNumItems() + 1);
    presetSelector.addItem("Through the Looking Glass", presetSelector.getNumItems() + 1);

//    presetSelector.onChange = [this] { processorRef.loadPresetProgram(presetSelector.getSelectedItemIndex()); };
    presetSelector.onChange = [this] {
        
        if( selectedPreset != presetSelector.getSelectedItemIndex() )
        {
            selectedPreset = presetSelector.getSelectedItemIndex();
            processorRef.currentProgramNumber = selectedPreset;
            callFactoryPreset(selectedPreset);
        };
        
        
    };

    //presetSelector.setSelectedItemIndex (selectedPreset);
    reloadState();
    
}

void AudioPluginAudioProcessorEditor::reloadState()
{
    if(processorRef.currentProgramNumber<10){
        selectedPreset = processorRef.currentProgramNumber;
        presetSelector.setSelectedItemIndex (processorRef.currentProgramNumber);
    }
    
    
    if(processorRef.diffusionEnabledButtonValue>0.5f){
        toggleDiffusionEnabled(true);
    }else{
        toggleDiffusionEnabled(false);
    }
    
    if(processorRef.lateDiffusionEnabledButtonValue>0.5f){
        toggleLateDiffusionEnabled(true);
    }else{
        toggleLateDiffusionEnabled(false);
    }
    
    if(processorRef.hiPassEnabledButtonValue>0.5f){
        toggleHighPassEnabled(true);
    }else{
        toggleHighPassEnabled(false);
    }
    
    if(processorRef.lowPassEnabledButtonValue>0.5f){
        toggleLowPassEnabled(true);
    }else{
        toggleLowPassEnabled(false);
    }
    
    if(processorRef.lowShelfEnabledButtonValue>0.5f){
        toggleLowShelfEnabled(true);
    }else{
        toggleLowShelfEnabled(false);
    }
    
    if(processorRef.highShelfEnabledButtonValue>0.5f){
        toggleHighShelfEnabled(true);
    }else{
        toggleHighShelfEnabled(false);
    }
    
    if(processorRef.cutoffEnabledButtonValue>0.5f){
        toggleCutoffEnabled(true);
    }else{
        toggleCutoffEnabled(false);
    }
    
    if(processorRef.lateStageTapButtonValue>0.5f){
        toggleLateStageTap(true);
    }else{
        toggleLateStageTap(false);
    }
    
    if(processorRef.interpolationButtonValue>0.5f){
        toggleInterpolation(true);
    }else{
        toggleInterpolation(false);
    }
}



void AudioPluginAudioProcessorEditor::toggleDiffusionEnabled(bool on){
    if(on){
        diffusionEnabledButton.setToggleState(true, juce::NotificationType::dontSendNotification);
        diffusionEnabledButton.setButtonText("On");
    } else {
        diffusionEnabledButton.setToggleState(false, juce::NotificationType::dontSendNotification);
        diffusionEnabledButton.setButtonText("Off");
    }
}
void AudioPluginAudioProcessorEditor::toggleLateDiffusionEnabled(bool on){
    if(on){
        lateDiffusionEnabledButton.setToggleState(true, juce::NotificationType::dontSendNotification);
        lateDiffusionEnabledButton.setButtonText("On");
    } else {
        lateDiffusionEnabledButton.setToggleState(false, juce::NotificationType::dontSendNotification);
        lateDiffusionEnabledButton.setButtonText("Off");
    }
}
void AudioPluginAudioProcessorEditor::toggleHighPassEnabled(bool on){
    if(on){
        hiPassEnabledButton.setToggleState(true, juce::NotificationType::dontSendNotification);
        hiPassEnabledButton.setButtonText("On");
    } else {
        hiPassEnabledButton.setToggleState(false, juce::NotificationType::dontSendNotification);
        hiPassEnabledButton.setButtonText("Off");
    }
}
void AudioPluginAudioProcessorEditor::toggleLowPassEnabled(bool on){
    if(on){
        lowPassEnabledButton.setToggleState(true, juce::NotificationType::dontSendNotification);
        lowPassEnabledButton.setButtonText("On");
    } else {
        lowPassEnabledButton.setToggleState(false, juce::NotificationType::dontSendNotification);
        lowPassEnabledButton.setButtonText("Off");
    }
}
void AudioPluginAudioProcessorEditor::toggleLowShelfEnabled(bool on){
    if(on){
        lowShelfEnabledButton.setToggleState(true, juce::NotificationType::dontSendNotification);
        lowShelfEnabledButton.setButtonText("On");
    } else {
        lowShelfEnabledButton.setToggleState(false, juce::NotificationType::dontSendNotification);
        lowShelfEnabledButton.setButtonText("Off");
    }
}
void AudioPluginAudioProcessorEditor::toggleHighShelfEnabled(bool on){
    if(on){
        highShelfEnabledButton.setToggleState(true, juce::NotificationType::dontSendNotification);
        highShelfEnabledButton.setButtonText("On");
    } else {
        highShelfEnabledButton.setToggleState(false, juce::NotificationType::dontSendNotification);
        highShelfEnabledButton.setButtonText("Off");
    }
}
void AudioPluginAudioProcessorEditor::toggleCutoffEnabled(bool on){
    if(on){
        cutoffEnabledButton.setToggleState(true, juce::NotificationType::dontSendNotification);
        cutoffEnabledButton.setButtonText("On");
    } else {
        cutoffEnabledButton.setToggleState(false, juce::NotificationType::dontSendNotification);
        cutoffEnabledButton.setButtonText("Off");
    }
}
void AudioPluginAudioProcessorEditor::toggleLateStageTap(bool on){
    if(on){
        lateStageTapButton.setToggleState(true, juce::NotificationType::dontSendNotification);
        lateStageTapButton.setButtonText("Pre");
    } else {
        lateStageTapButton.setToggleState(false, juce::NotificationType::dontSendNotification);
        lateStageTapButton.setButtonText("Post");
    }
}
void AudioPluginAudioProcessorEditor::toggleInterpolation(bool on){
    if(on){
        interpolationButton.setToggleState(true, juce::NotificationType::dontSendNotification);
        interpolationButton.setButtonText("On");
    } else {
        interpolationButton.setToggleState(false, juce::NotificationType::dontSendNotification);
        interpolationButton.setButtonText("Off");
    }
}



void AudioPluginAudioProcessorEditor::callFactoryPreset(int index)
{
    switch(index){
        case 0:
            inputMixSlider.setValue(0.0f);
            predelaySlider.setValue(0.070000000298023224*1000.0);
            lowCutSlider.setValue(0.0f);
            highCutSlider.setValue(0.29000008106231689);
            dryOutSlider.setValue(0.94499987363815308f);
            predelayOutSlider.setValue(0.0f);
            earlyOutSlider.setValue(0.77999997138977051f);
            mainOutSlider.setValue(0.74500006437301636f);
            tapCountSlider.setValue(0.36499997973442078);
            tapLengthSlider.setValue(500.0f);
            diffusionDelaySlider.setValue(0.43500006198883057);
            earlyDiffusionModAmountSlider.setValue(0.50000005960464478*2.5);
            tapGainSlider.setValue(1.0f);
            tapDecaySlider.setValue(0.86500012874603271);
            diffusionFeedbackSlider.setValue(0.725000262260437);
            earlyDiffusionModRateSlider.setValue(0.42500010132789612);
            lineDelaySlider.setValue(0.68499988317489624f);
            lineModAmountSlider.setValue(0.59000003337860107f*2.5);
            lateDiffusionDelaySlider.setValue(0.68499988317489624f);
            lateDiffusionModAmountSlider.setValue(0.619999885559082f*2.5);
            lineDecaySlider.setValue(0.68000012636184692f);
            lineModRateSlider.setValue(0.46999993920326233f);
            lateDiffusionFeedbackSlider.setValue(0.65999996662139893f);
            lateDiffusionModRateSlider.setValue(0.42500019073486328f);
            postLowShelfFrequencySlider.setValue(0.31499990820884705f);
            postHighShelfFrequencySlider.setValue(0.73000013828277588f);
            postCutoffFrequencySlider.setValue(0.73499983549118042f);
            postLowShelfGainSlider.setValue(0.5199999213218689f);
            postHighShelfGainSlider.setValue(0.83500003814697266f);
            crossSeedSlider.setValue(0.0f);
            tapSeedSlider.setValue(0.0011500000255182385 * 1000000 + 0.001);
            diffusionSeedSlider.setValue(0.00018899999849963933 * 1000000 + 0.001);
            delaySeedSlider.setValue(0.00033700000494718552 * 1000000 + 0.001);
            postDiffusionSeedSlider.setValue(0.00050099997315555811 * 1000000 + 0.001);
            lineCountSlider.setValue(12);
            diffusionStagesSlider.setValue(4);
            lateDiffusionStagesSlider.setValue(3);
            toggleDiffusionEnabled(true);
            toggleLateDiffusionEnabled(true);
            toggleHighPassEnabled(false);
            toggleLowPassEnabled(false);
            toggleCutoffEnabled(true);
            toggleLowShelfEnabled(false);
            toggleHighShelfEnabled(false);
            toggleLateStageTap(true);
            toggleInterpolation(false);
            processorRef.diffusionEnabledButtonValue = 1.0f;
            processorRef.lateDiffusionEnabledButtonValue = 1.0f;
            processorRef.hiPassEnabledButtonValue = 0.0f;
            processorRef.lowPassEnabledButtonValue = 0.0f;
            processorRef.cutoffEnabledButtonValue = 1.0f;
            processorRef.lowShelfEnabledButtonValue = 0.0f;
            processorRef.highShelfEnabledButtonValue = 0.0f;
            processorRef.lateStageTapButtonValue = 1.0f;
            processorRef.interpolationButtonValue = 0.0f;
            break;
        case 1:
            
            //parameters from Dull Echos in
            //https://github.com/ValdemarOrn/CloudSeed/tree/master/Factory%20Programs
            inputMixSlider.setValue(0.0);
            predelaySlider.setValue(0.070000000298023224*1000.0);
            lowCutSlider.setValue(0.0);
            highCutSlider.setValue(0.29000008106231689);
            tapCountSlider.setValue(0.36499997973442078);
            tapLengthSlider.setValue(500.0);
            tapGainSlider.setValue(0.83499991893768311);
            tapDecaySlider.setValue(0.86500012874603271);
            diffusionStagesSlider.setValue(4);
            diffusionDelaySlider.setValue(0.43500006198883057);
            diffusionFeedbackSlider.setValue(0.725000262260437);
            lineCountSlider.setValue(12);
            lineDelaySlider.setValue(0.34500002861022949);
            lineDecaySlider.setValue(0.41500008106231689);
            lateDiffusionStagesSlider.setValue(5);
            lateDiffusionDelaySlider.setValue(0.66499996185302734);
            lateDiffusionFeedbackSlider.setValue(0.61000001430511475);
            postLowShelfGainSlider.setValue(0.5199999213218689);
            postLowShelfFrequencySlider.setValue(0.31499990820884705);
            postHighShelfGainSlider.setValue(0.83500003814697266);
            postHighShelfFrequencySlider.setValue(0.73000013828277588);
            postCutoffFrequencySlider.setValue(0.73499983549118042);
            earlyDiffusionModAmountSlider.setValue(0.25499999523162842*2.5);
            earlyDiffusionModRateSlider.setValue(0.3250001072883606);
            lineModAmountSlider.setValue(0.33500000834465027*2.5);
            lineModRateSlider.setValue(0.26999998092651367);
            lateDiffusionModAmountSlider.setValue(0.13499975204467773*2.5);
            lateDiffusionModRateSlider.setValue(0.27500006556510925);
            tapSeedSlider.setValue(0.0011500000255182385 * 1000000 + 0.001);
            diffusionSeedSlider.setValue(0.00018899999849963933 * 1000000 + 0.001);
            delaySeedSlider.setValue(0.0002730000123847276 * 1000000 + 0.001);
            postDiffusionSeedSlider.setValue(0.00050099997315555811 * 1000000 + 0.001);
            crossSeedSlider.setValue(0.5);
            dryOutSlider.setValue(1.0);
            predelayOutSlider.setValue(0.0);
            earlyOutSlider.setValue(0.77999997138977051);
            mainOutSlider.setValue(0.74500006437301636);
            toggleDiffusionEnabled(true);
            toggleLateDiffusionEnabled(false);
            toggleHighPassEnabled(false);
            toggleLowPassEnabled(true);
            toggleCutoffEnabled(true);
            toggleLowShelfEnabled(false);
            toggleHighShelfEnabled(false);
            toggleLateStageTap(false);
            toggleInterpolation(true);
            processorRef.diffusionEnabledButtonValue = 1.0f;
            processorRef.lateDiffusionEnabledButtonValue = 0.0f;
            processorRef.hiPassEnabledButtonValue = 0.0f;
            processorRef.lowPassEnabledButtonValue = 1.0f;
            processorRef.cutoffEnabledButtonValue = 1.0f;
            processorRef.lowShelfEnabledButtonValue = 0.0f;
            processorRef.highShelfEnabledButtonValue = 0.0f;
            processorRef.lateStageTapButtonValue = 0.0f;
            processorRef.interpolationButtonValue = 1.0f;
            break;
        case 2:
            //parameters from Hyperplane in
            //https://github.com/ValdemarOrn/CloudSeed/tree/master/Factory%20Programs
            inputMixSlider.setValue(0.1549999862909317);
            predelaySlider.setValue(0.0);
            lowCutSlider.setValue(0.57999998331069946);
            highCutSlider.setValue(0.9100000262260437);
            tapCountSlider.setValue(0.41499990224838257);
            tapLengthSlider.setValue(220.0);
            tapGainSlider.setValue(1.0);
            tapDecaySlider.setValue(1.0);
            diffusionStagesSlider.setValue(4);
            diffusionDelaySlider.setValue(0.27500024437904358);
            diffusionFeedbackSlider.setValue(0.660000205039978);
            lineCountSlider.setValue(9);
            lineDelaySlider.setValue(0.22500017285346985);
            lineDecaySlider.setValue(0.794999897480011);
            lateDiffusionStagesSlider.setValue(8);
            lateDiffusionDelaySlider.setValue(0.22999951243400574);
            lateDiffusionFeedbackSlider.setValue(0.59499990940093994);
            postLowShelfGainSlider.setValue(0.95999979972839355);
            postLowShelfFrequencySlider.setValue(0.23999994993209839);
            postHighShelfGainSlider.setValue(0.97500002384185791);
            postHighShelfFrequencySlider.setValue(0.78499996662139893);
            postCutoffFrequencySlider.setValue(0.87999981641769409);
            earlyDiffusionModAmountSlider.setValue(0.13499999046325684*2.5);
            earlyDiffusionModRateSlider.setValue(0.29000008106231689);
            lineModAmountSlider.setValue(0.53999996185302734*2.5);
            lineModRateSlider.setValue(0.44999989867210388);
            lateDiffusionModAmountSlider.setValue(0.15999998152256012*2.5);
            lateDiffusionModRateSlider.setValue(0.56000012159347534);
            tapSeedSlider.setValue(0.00048499999684281647 * 1000000 + 0.001);
            diffusionSeedSlider.setValue(0.00020799999765586108 * 1000000 + 0.001);
            delaySeedSlider.setValue(0.00034699999378062785 * 1000000 + 0.001);
            postDiffusionSeedSlider.setValue(0.00037200000951997936 * 1000000 + 0.001);
            crossSeedSlider.setValue(0.800000011920929);
            dryOutSlider.setValue(0.86500018835067749);
            predelayOutSlider.setValue(0.0);
            earlyOutSlider.setValue(0.8200000524520874);
            mainOutSlider.setValue(0.79500007629394531);
            toggleDiffusionEnabled(true);
            toggleLateDiffusionEnabled(true);
            toggleHighPassEnabled(true);
            toggleLowPassEnabled(true);
            toggleCutoffEnabled(true);
            toggleLowShelfEnabled(true);
            toggleHighShelfEnabled(true);
            toggleLateStageTap(true);
            toggleInterpolation(false);
            processorRef.diffusionEnabledButtonValue = 1.0f;
            processorRef.lateDiffusionEnabledButtonValue = 1.0f;
            processorRef.hiPassEnabledButtonValue = 1.0f;
            processorRef.lowPassEnabledButtonValue = 1.0f;
            processorRef.cutoffEnabledButtonValue = 1.0f;
            processorRef.lowShelfEnabledButtonValue = 1.0f;
            processorRef.highShelfEnabledButtonValue = 1.0f;
            processorRef.lateStageTapButtonValue = 1.0f;
            processorRef.interpolationButtonValue = 0.0f;
            break;
        case 3:
            //parameters from Medium Space in
            //https://github.com/ValdemarOrn/CloudSeed/tree/master/Factory%20Programs
            inputMixSlider.setValue(0.0);
            predelaySlider.setValue(0.0);
            lowCutSlider.setValue(0.0);
            highCutSlider.setValue(0.63999992609024048);
            tapCountSlider.setValue(0.51999980211257935);
            tapLengthSlider.setValue(132.5);
            tapGainSlider.setValue(0.69499999284744263);
            tapDecaySlider.setValue(1.0);
            diffusionStagesSlider.setValue(7);
            diffusionDelaySlider.setValue(0.5700000524520874);
            diffusionFeedbackSlider.setValue(0.76000010967254639);
            lineCountSlider.setValue(3);
            lineDelaySlider.setValue(0.585000216960907);
            lineDecaySlider.setValue(0.29499980807304382);
            lateDiffusionStagesSlider.setValue(5);
            lateDiffusionDelaySlider.setValue(0.69499951601028442);
            lateDiffusionFeedbackSlider.setValue(0.71499985456466675);
            postLowShelfGainSlider.setValue(0.87999987602233887);
            postLowShelfFrequencySlider.setValue(0.19499993324279785);
            postHighShelfGainSlider.setValue(0.72000008821487427);
            postHighShelfFrequencySlider.setValue(0.520000159740448);
            postCutoffFrequencySlider.setValue(0.79999983310699463);
            earlyDiffusionModAmountSlider.setValue(0.13499999046325684*2.5);
            earlyDiffusionModRateSlider.setValue(0.26000010967254639);
            lineModAmountSlider.setValue(0.054999928921461105*2.5);
            lineModRateSlider.setValue(0.21499986946582794);
            lateDiffusionModAmountSlider.setValue(0.17999963462352753*2.5);
            lateDiffusionModRateSlider.setValue(0.38000011444091797);
            tapSeedSlider.setValue(0.0003009999927598983 * 1000000 + 0.001);
            diffusionSeedSlider.setValue(0.00018899999849963933 * 1000000 + 0.001);
            delaySeedSlider.setValue(0.0001610000035725534 * 1000000 + 0.001);
            postDiffusionSeedSlider.setValue(0.00050099997315555811 * 1000000 + 0.001);
            crossSeedSlider.setValue(0.7850000262260437);
            dryOutSlider.setValue(1.0);
            predelayOutSlider.setValue(0.0);
            earlyOutSlider.setValue(0.699999988079071);
            mainOutSlider.setValue(0.84499984979629517);
            toggleDiffusionEnabled(true);
            toggleLateDiffusionEnabled(true);
            toggleHighPassEnabled(false);
            toggleLowPassEnabled(true);
            toggleCutoffEnabled(true);
            toggleLowShelfEnabled(true);
            toggleHighShelfEnabled(false);
            toggleLateStageTap(true);
            toggleInterpolation(true);
            processorRef.diffusionEnabledButtonValue = 1.0f;
            processorRef.lateDiffusionEnabledButtonValue = 1.0f;
            processorRef.hiPassEnabledButtonValue = 0.0f;
            processorRef.lowPassEnabledButtonValue = 1.0f;
            processorRef.cutoffEnabledButtonValue = 1.0f;
            processorRef.lowShelfEnabledButtonValue = 1.0f;
            processorRef.highShelfEnabledButtonValue = 0.0f;
            processorRef.lateStageTapButtonValue = 1.0f;
            processorRef.interpolationButtonValue = 1.0f;
            break;
        case 4:
            //parameters from Noise In The Hallway in
            //https://github.com/ValdemarOrn/CloudSeed/tree/master/Factory%20Programs
            inputMixSlider.setValue(0.0);
            predelaySlider.setValue(0.0);
            lowCutSlider.setValue(0.0);
            highCutSlider.setValue(0.60999995470047);
            tapCountSlider.setValue(1.0);
            tapLengthSlider.setValue(500.0);
            tapGainSlider.setValue(0.0);
            tapDecaySlider.setValue(0.830000102519989);
            diffusionStagesSlider.setValue(3);
            diffusionDelaySlider.setValue(0.35499998927116394);
            diffusionFeedbackSlider.setValue(0.62500005960464478);
            lineCountSlider.setValue(8);
            lineDelaySlider.setValue(0.36000004410743713);
            lineDecaySlider.setValue(0.51000005006790161);
            lateDiffusionStagesSlider.setValue(1);
            lateDiffusionDelaySlider.setValue(0.62999987602233887);
            lateDiffusionFeedbackSlider.setValue(0.49000000953674316);
            postLowShelfGainSlider.setValue(0.0);
            postLowShelfFrequencySlider.setValue(0.0);
            postHighShelfGainSlider.setValue(0.77499985694885254);
            postHighShelfFrequencySlider.setValue(0.58000004291534424);
            postCutoffFrequencySlider.setValue(0.0);
            earlyDiffusionModAmountSlider.setValue(0.0);
            earlyDiffusionModRateSlider.setValue(0.0);
            lineModAmountSlider.setValue(0.0);
            lineModRateSlider.setValue(0.0);
            lateDiffusionModAmountSlider.setValue(0.0);
            lateDiffusionModRateSlider.setValue(0.0);
            tapSeedSlider.setValue(0.0001140000022132881 * 1000000 + 0.001);
            diffusionSeedSlider.setValue(0.000155999994603917 * 1000000 + 0.001);
            delaySeedSlider.setValue(0.00018099999579135329 * 1000000 + 0.001);
            postDiffusionSeedSlider.setValue(8.4999999671708792E-05 * 1000000 + 0.001);
            crossSeedSlider.setValue(1.0);
            dryOutSlider.setValue(0.0);
            predelayOutSlider.setValue(0.0);
            earlyOutSlider.setValue(0.64500010013580322);
            mainOutSlider.setValue(0.63000005483627319);
            toggleDiffusionEnabled(true);
            toggleLateDiffusionEnabled(true);
            toggleHighPassEnabled(false);
            toggleLowPassEnabled(true);
            toggleCutoffEnabled(false);
            toggleLowShelfEnabled(false);
            toggleHighShelfEnabled(true);
            toggleLateStageTap(false);
            toggleInterpolation(true);
            processorRef.diffusionEnabledButtonValue = 1.0f;
            processorRef.lateDiffusionEnabledButtonValue = 1.0f;
            processorRef.hiPassEnabledButtonValue = 0.0f;
            processorRef.lowPassEnabledButtonValue = 1.0f;
            processorRef.cutoffEnabledButtonValue = 0.0f;
            processorRef.lowShelfEnabledButtonValue = 0.0f;
            processorRef.highShelfEnabledButtonValue = 1.0f;
            processorRef.lateStageTapButtonValue = 0.0f;
            processorRef.interpolationButtonValue = 1.0f;
            break;
        case 5:
            //parameters from Rubi-Ka Fields in
              //https://github.com/ValdemarOrn/CloudSeed/tree/master/Factory%20Programs
              inputMixSlider.setValue(0.32499998807907104);
              predelaySlider.setValue(0.0);
              lowCutSlider.setValue(0.0);
              highCutSlider.setValue(0.8899998664855957);
              tapCountSlider.setValue(0.51999980211257935);
              tapLengthSlider.setValue(500.0);
              tapGainSlider.setValue(0.90000003576278687);
              tapDecaySlider.setValue(1.0);
              diffusionStagesSlider.setValue(7);
              diffusionDelaySlider.setValue(0.5700000524520874);
              diffusionFeedbackSlider.setValue(0.76000010967254639);
              lineCountSlider.setValue(4);
              lineDelaySlider.setValue(0.68500018119812012);
              lineDecaySlider.setValue(0.82999974489212036);
              lateDiffusionStagesSlider.setValue(6);
              lateDiffusionDelaySlider.setValue(0.69499951601028442);
              lateDiffusionFeedbackSlider.setValue(0.71499985456466675);
              postLowShelfGainSlider.setValue(0.87999987602233887);
              postLowShelfFrequencySlider.setValue(0.19499993324279785);
              postHighShelfGainSlider.setValue(0.72000008821487427);
              postHighShelfFrequencySlider.setValue(0.520000159740448);
              postCutoffFrequencySlider.setValue(0.79999983310699463);
              earlyDiffusionModAmountSlider.setValue(0.13499999046325684*2.5);
              earlyDiffusionModRateSlider.setValue(0.26000010967254639);
              lineModAmountSlider.setValue(0.054999928921461105*2.5);
              lineModRateSlider.setValue(0.21499986946582794);
              lateDiffusionModAmountSlider.setValue(0.32499963045120239*2.5);
              lateDiffusionModRateSlider.setValue(0.35500010848045349);
              tapSeedSlider.setValue(0.0003009999927598983 * 1000000 + 0.001);
              diffusionSeedSlider.setValue(0.00018899999849963933 * 1000000 + 0.001);
              delaySeedSlider.setValue(0.0001610000035725534 * 1000000 + 0.001);
              postDiffusionSeedSlider.setValue(0.00050099997315555811 * 1000000 + 0.001);
              crossSeedSlider.setValue(0.43000003695487976);
              dryOutSlider.setValue(0.88499999046325684);
              predelayOutSlider.setValue(0.0);
              earlyOutSlider.setValue(0.0);
              mainOutSlider.setValue(0.90999990701675415);
            toggleDiffusionEnabled(true);
            toggleLateDiffusionEnabled(true);
            toggleHighPassEnabled(false);
            toggleLowPassEnabled(false);
            toggleCutoffEnabled(true);
            toggleLowShelfEnabled(false);
            toggleHighShelfEnabled(false);
            toggleLateStageTap(true);
            toggleInterpolation(false);
            processorRef.diffusionEnabledButtonValue = 1.0f;
            processorRef.lateDiffusionEnabledButtonValue = 1.0f;
            processorRef.hiPassEnabledButtonValue = 0.0f;
            processorRef.lowPassEnabledButtonValue = 0.0f;
            processorRef.cutoffEnabledButtonValue = 1.0f;
            processorRef.lowShelfEnabledButtonValue = 0.0f;
            processorRef.highShelfEnabledButtonValue = 0.0f;
            processorRef.lateStageTapButtonValue = 1.0f;
            processorRef.interpolationButtonValue = 0.0f;
            break;
        case 6:
            //parameters from Small Room in
            //https://github.com/ValdemarOrn/CloudSeed/tree/master/Factory%20Programs
            inputMixSlider.setValue(0.0);
            predelaySlider.setValue(0.0);
            lowCutSlider.setValue(0.0);
            highCutSlider.setValue(0.755000114440918);
            tapCountSlider.setValue(0.41499990224838257);
            tapLengthSlider.setValue(220.0);
            tapGainSlider.setValue(0.87999999523162842);
            tapDecaySlider.setValue(1.0);
            diffusionStagesSlider.setValue(6);
            diffusionDelaySlider.setValue(0.335000216960907);
            diffusionFeedbackSlider.setValue(0.660000205039978);
            lineCountSlider.setValue(3);
            lineDelaySlider.setValue(0.51000016927719116);
            lineDecaySlider.setValue(0.29999998211860657);
            lateDiffusionStagesSlider.setValue(4);
            lateDiffusionDelaySlider.setValue(0.22999951243400574);
            lateDiffusionFeedbackSlider.setValue(0.59499990940093994);
            postLowShelfGainSlider.setValue(0.87999987602233887);
            postLowShelfFrequencySlider.setValue(0.19499993324279785);
            postHighShelfGainSlider.setValue(0.875);
            postHighShelfFrequencySlider.setValue(0.59000009298324585);
            postCutoffFrequencySlider.setValue(0.79999983310699463);
            earlyDiffusionModAmountSlider.setValue(0.13499999046325684*2.5);
            earlyDiffusionModRateSlider.setValue(0.29000008106231689);
            lineModAmountSlider.setValue(0.18999995291233063*2.5);
            lineModRateSlider.setValue(0.22999987006187439);
            lateDiffusionModAmountSlider.setValue(0.1249999925494194*2.5);
            lateDiffusionModRateSlider.setValue(0.28500008583068848);
            tapSeedSlider.setValue(0.00048499999684281647 * 1000000 + 0.001);
            diffusionSeedSlider.setValue(0.00020799999765586108 * 1000000 + 0.001);
            delaySeedSlider.setValue(0.00033499998971819878 * 1000000 + 0.001);
            postDiffusionSeedSlider.setValue(0.00037200000951997936 * 1000000 + 0.001);
            crossSeedSlider.setValue(0.42500001192092896);
            dryOutSlider.setValue(1.0);
            predelayOutSlider.setValue(0.0);
            earlyOutSlider.setValue(0.8599998950958252);
            mainOutSlider.setValue(0.90500003099441528);
            toggleDiffusionEnabled(true);
            toggleLateDiffusionEnabled(true);
            toggleHighPassEnabled(false);
            toggleLowPassEnabled(true);
            toggleCutoffEnabled(false);
            toggleLowShelfEnabled(false);
            toggleHighShelfEnabled(false);
            toggleLateStageTap(true);
            toggleInterpolation(true);
            processorRef.diffusionEnabledButtonValue = 1.0f;
            processorRef.lateDiffusionEnabledButtonValue = 1.0f;
            processorRef.hiPassEnabledButtonValue = 0.0f;
            processorRef.lowPassEnabledButtonValue = 1.0f;
            processorRef.cutoffEnabledButtonValue = 0.0f;
            processorRef.lowShelfEnabledButtonValue = 0.0f;
            processorRef.highShelfEnabledButtonValue = 0.0f;
            processorRef.lateStageTapButtonValue = 1.0f;
            processorRef.interpolationButtonValue = 1.0f;
            break;
        case 7:
            //parameters from The 90s Are Back in
            //https://github.com/ValdemarOrn/CloudSeed/tree/master/Factory%20Programs
            inputMixSlider.setValue(0);
            predelaySlider.setValue(0);
            lowCutSlider.setValue(0);
            highCutSlider.setValue(0.6750001311302185);
            tapCountSlider.setValue(0);
            tapLengthSlider.setValue(500.0);
            tapGainSlider.setValue(0);
            tapDecaySlider.setValue(0.8650001287460327);
            diffusionStagesSlider.setValue(5);
            diffusionDelaySlider.setValue(0.7100000381469727);
            diffusionFeedbackSlider.setValue(0.5450003147125244);
            lineCountSlider.setValue(9);
            lineDelaySlider.setValue(0.6849998831748962);
            lineDecaySlider.setValue(0.6300000548362732);
            lateDiffusionStagesSlider.setValue(3);
            lateDiffusionDelaySlider.setValue(0.5449999570846558);
            lateDiffusionFeedbackSlider.setValue(0.6599999666213989);
            postLowShelfGainSlider.setValue(0.5199999213218689);
            postLowShelfFrequencySlider.setValue(0.31499990820884705);
            postHighShelfGainSlider.setValue(0.8349999189376831);
            postHighShelfFrequencySlider.setValue(0.705000102519989);
            postCutoffFrequencySlider.setValue(0.7349998354911804);
            earlyDiffusionModAmountSlider.setValue(0.824999988079071*2.5);
            earlyDiffusionModRateSlider.setValue(0.4050004780292511);
            lineModAmountSlider.setValue(0.6300000548362732*2.5);
            lineModRateSlider.setValue(0.3199999928474426);
            lateDiffusionModAmountSlider.setValue(0.619999885559082*2.5);
            lateDiffusionModRateSlider.setValue(0.30000022053718567);
            tapSeedSlider.setValue(0.0011500000255182385 * 1000000 + 0.001);
            diffusionSeedSlider.setValue(0.00018899999849963933 * 1000000 + 0.001);
            delaySeedSlider.setValue(0.0003370000049471855 * 1000000 + 0.001);
            postDiffusionSeedSlider.setValue(0.0005009999731555581 * 1000000 + 0.001);
            crossSeedSlider.setValue(0.7950000166893005);
            dryOutSlider.setValue(0.9449997544288635);
            predelayOutSlider.setValue(0);
            earlyOutSlider.setValue(0.7250000238418579);
            mainOutSlider.setValue(0.6050001382827759);
            toggleDiffusionEnabled(true);
            toggleLateDiffusionEnabled(false);
            toggleHighPassEnabled(false);
            toggleLowPassEnabled(true);
            toggleCutoffEnabled(false);
            toggleLowShelfEnabled(false);
            toggleHighShelfEnabled(true);
            toggleLateStageTap(true);
            toggleInterpolation(true);
            processorRef.diffusionEnabledButtonValue = 1.0f;
            processorRef.lateDiffusionEnabledButtonValue = 0.0f;
            processorRef.hiPassEnabledButtonValue = 0.0f;
            processorRef.lowPassEnabledButtonValue = 1.0f;
            processorRef.cutoffEnabledButtonValue = 0.0f;
            processorRef.lowShelfEnabledButtonValue = 0.0f;
            processorRef.highShelfEnabledButtonValue = 1.0f;
            processorRef.lateStageTapButtonValue = 1.0f;
            processorRef.interpolationButtonValue = 1.0f;
            break;
        case 8:
            //parameters from Through The Looking Glass in
            //https://github.com/ValdemarOrn/CloudSeed/tree/master/Factory%20Programs
            inputMixSlider.setValue(0.0);
            predelaySlider.setValue(0.0);
            lowCutSlider.setValue(0.0);
            highCutSlider.setValue(0.74000012874603271);
            tapCountSlider.setValue(1.0);
            tapLengthSlider.setValue(500.0);
            tapGainSlider.setValue(1.0);
            tapDecaySlider.setValue(0.71000003814697266);
            diffusionStagesSlider.setValue(8);
            diffusionDelaySlider.setValue(0.65999996662139893);
            diffusionFeedbackSlider.setValue(0.76000010967254639);
            lineCountSlider.setValue(12);
            lineDelaySlider.setValue(0.9100002646446228);
            lineDecaySlider.setValue(0.80999958515167236);
            lateDiffusionStagesSlider.setValue(8);
            lateDiffusionDelaySlider.setValue(0.71499955654144287);
            lateDiffusionFeedbackSlider.setValue(0.71999979019165039);
            postLowShelfGainSlider.setValue(0.87999987602233887);
            postLowShelfFrequencySlider.setValue(0.19499993324279785);
            postHighShelfGainSlider.setValue(0.72000008821487427);
            postHighShelfFrequencySlider.setValue(0.520000159740448);
            postCutoffFrequencySlider.setValue(0.7150002121925354);
            earlyDiffusionModAmountSlider.setValue(0.41999998688697815*2.5);
            earlyDiffusionModRateSlider.setValue(0.30500012636184692);
            lineModAmountSlider.setValue(0.4649999737739563*2.5);
            lineModRateSlider.setValue(0.3199998140335083);
            lateDiffusionModAmountSlider.setValue(0.40999993681907654*2.5);
            lateDiffusionModRateSlider.setValue(0.31500011682510376);
            tapSeedSlider.setValue(0.0003009999927598983 * 1000000 + 0.001);
            diffusionSeedSlider.setValue(0.00018899999849963933 * 1000000 + 0.001);
            delaySeedSlider.setValue(0.0001610000035725534 * 1000000 + 0.001);
            postDiffusionSeedSlider.setValue(0.00050099997315555811 * 1000000 + 0.001);
            crossSeedSlider.setValue(1.0);
            dryOutSlider.setValue(0.0);
            predelayOutSlider.setValue(0.0);
            earlyOutSlider.setValue(0.0);
            mainOutSlider.setValue(0.95499974489212036);
            toggleDiffusionEnabled(true);
            toggleLateDiffusionEnabled(true);
            toggleHighPassEnabled(false);
            toggleLowPassEnabled(true);
            toggleCutoffEnabled(true);
            toggleLowShelfEnabled(false);
            toggleHighShelfEnabled(false);
            toggleLateStageTap(true);
            toggleInterpolation(true);
            processorRef.diffusionEnabledButtonValue = 1.0f;
            processorRef.lateDiffusionEnabledButtonValue = 1.0f;
            processorRef.hiPassEnabledButtonValue = 0.0f;
            processorRef.lowPassEnabledButtonValue = 1.0f;
            processorRef.cutoffEnabledButtonValue = 1.0f;
            processorRef.lowShelfEnabledButtonValue = 0.0f;
            processorRef.highShelfEnabledButtonValue = 0.0f;
            processorRef.lateStageTapButtonValue = 1.0f;
            processorRef.interpolationButtonValue = 1.0f;
            break;
        default:
            break;
    }
}


void AudioPluginAudioProcessorEditor::buttonClicked (juce::Button* button)
{
    if(button == &diffusionEnabledButton)
    {
        auto newVal = processorRef.diffusionEnabledButtonValue < 0.5f ? 1.0f : 0.0f;
        processorRef.diffusionEnabledButtonValue = newVal;
        if(newVal > 0.5f)   toggleDiffusionEnabled(true);
        else                toggleDiffusionEnabled(false);
    }
    else if(button == &lateDiffusionEnabledButton)
    {
        auto newVal = processorRef.lateDiffusionEnabledButtonValue < 0.5f ? 1.0f : 0.0f;
        processorRef.lateDiffusionEnabledButtonValue = newVal;
        if(newVal > 0.5f)   toggleLateDiffusionEnabled(true);
        else                toggleLateDiffusionEnabled(false);
    }
    else if(button == &hiPassEnabledButton)
    {
        auto newVal = processorRef.hiPassEnabledButtonValue < 0.5f ? 1.0f : 0.0f;
        processorRef.hiPassEnabledButtonValue = newVal;
        if(newVal > 0.5f)   toggleHighPassEnabled(true);
        else                toggleHighPassEnabled(false);
    }
    else if(button == &lowPassEnabledButton)
    {
        auto newVal = processorRef.lowPassEnabledButtonValue < 0.5f ? 1.0f : 0.0f;
        processorRef.lowPassEnabledButtonValue = newVal;
        if(newVal > 0.5f)   toggleLowPassEnabled(true);
        else                toggleLowPassEnabled(false);
    }
    
    else if(button == &lowShelfEnabledButton)
    {
        auto newVal = processorRef.lowShelfEnabledButtonValue < 0.5f ? 1.0f : 0.0f;
        processorRef.lowShelfEnabledButtonValue = newVal;
        if(newVal > 0.5f)   toggleLowShelfEnabled(true);
        else                toggleLowShelfEnabled(false);
    }
    else if(button == &highShelfEnabledButton)
    {
        auto newVal = processorRef.highShelfEnabledButtonValue < 0.5f ? 1.0f : 0.0f;
        processorRef.highShelfEnabledButtonValue = newVal;
        if(newVal > 0.5f)   toggleHighShelfEnabled(true);
        else                toggleHighShelfEnabled(false);
    }
    
    else if(button == &cutoffEnabledButton)
    {
        auto newVal = processorRef.cutoffEnabledButtonValue < 0.5f ? 1.0f : 0.0f;
        processorRef.cutoffEnabledButtonValue = newVal;
        if(newVal > 0.5f)   toggleCutoffEnabled(true);
        else                toggleCutoffEnabled(false);
    }
    
    else if(button == &lateStageTapButton)
    {
        auto newVal = processorRef.lateStageTapButtonValue < 0.5f ? 1.0f : 0.0f;
        processorRef.lateStageTapButtonValue = newVal;
        if(newVal > 0.5f)   toggleLateStageTap(true);
        else                toggleLateStageTap(false);
    }
    
    else if(button == &interpolationButton)
    {
        auto newVal = processorRef.interpolationButtonValue < 0.5f ? 1.0f : 0.0f;
        processorRef.interpolationButtonValue = newVal;
        if(newVal > 0.5f)   toggleInterpolation(true);
        else                toggleInterpolation(false);
    }

    // Note: updateParameters() is called from processBlock() on the audio thread
}

void AudioPluginAudioProcessorEditor::sliderValueChanged (juce::Slider* slider)
{
    if(slider == &inputMixSlider) {
        processorRef.inputMixSliderValue = inputMixSlider.getValue();
    } else if (slider == &predelaySlider){
        processorRef.predelaySliderValue = predelaySlider.getValue()/1000.0;
    } else if (slider == &lowCutSlider){
        processorRef.lowCutSliderValue = lowCutSlider.getValue();
    } else if (slider == &highCutSlider){
        processorRef.highCutSliderValue = highCutSlider.getValue();
    }
    
    else if(slider == &dryOutSlider) {
        processorRef.dryOutSliderValue = dryOutSlider.getValue();
    } else if (slider == &predelayOutSlider){
        processorRef.predelayOutSliderValue = predelayOutSlider.getValue();
    } else if (slider == &earlyOutSlider){
        processorRef.earlyOutSliderValue = earlyOutSlider.getValue();
    } else if (slider == &mainOutSlider){
        processorRef.mainOutSliderValue = mainOutSlider.getValue();
    }
    
    
    else if(slider == &tapCountSlider) {
        processorRef.tapCountSliderValue = tapCountSlider.getValue();
    } else if (slider == &tapLengthSlider){
        processorRef.tapLengthSliderValue = tapLengthSlider.getValue()/500.0;
    } else if (slider == &diffusionDelaySlider){
        processorRef.diffusionDelaySliderValue = diffusionDelaySlider.getValue();
    } else if (slider == &earlyDiffusionModAmountSlider){
        processorRef.earlyDiffusionModAmountSliderValue = earlyDiffusionModAmountSlider.getValue()*0.4;
    }
    
    else if(slider == &tapGainSlider) {
        processorRef.tapGainSliderValue = tapGainSlider.getValue();
    } else if (slider == &tapDecaySlider){
        processorRef.tapDecaySliderValue = tapDecaySlider.getValue();
    } else if (slider == &diffusionFeedbackSlider){
        processorRef.diffusionFeedbackSliderValue = diffusionFeedbackSlider.getValue();
    } else if (slider == &earlyDiffusionModRateSlider){
        processorRef.earlyDiffusionModRateSliderValue = earlyDiffusionModRateSlider.getValue();
    }
    
    
    else if(slider == &lineDelaySlider) {
        processorRef.lineDelaySliderValue = lineDelaySlider.getValue();
    } else if (slider == &lineModAmountSlider){
        processorRef.lineModAmountSliderValue = lineModAmountSlider.getValue()*0.4;
    } else if (slider == &lateDiffusionDelaySlider){
        processorRef.lateDiffusionDelaySliderValue = lateDiffusionDelaySlider.getValue();
    } else if (slider == &lateDiffusionModAmountSlider){
        processorRef.lateDiffusionModAmountSliderValue = lateDiffusionModAmountSlider.getValue()*0.4;
    }
    
    else if(slider == &lineDecaySlider) {
        processorRef.lineDecaySliderValue = lineDecaySlider.getValue();
    } else if (slider == &lineModRateSlider){
        processorRef.lineModRateSliderValue = lineModRateSlider.getValue();
    } else if (slider == &lateDiffusionFeedbackSlider){
        processorRef.lateDiffusionFeedbackSliderValue = lateDiffusionFeedbackSlider.getValue();
    } else if (slider == &lateDiffusionModRateSlider){
        processorRef.lateDiffusionModRateSliderValue = lateDiffusionModRateSlider.getValue();
    }
    
    
    else if(slider == &postLowShelfFrequencySlider) {
        processorRef.postLowShelfFrequencySliderValue = postLowShelfFrequencySlider.getValue();
    } else if (slider == &postHighShelfFrequencySlider){
        processorRef.postHighShelfFrequencySliderValue = postHighShelfFrequencySlider.getValue();
    } else if (slider == &postCutoffFrequencySlider){
        processorRef.postCutoffFrequencySliderValue = postCutoffFrequencySlider.getValue();
    }
    
    else if(slider == &postLowShelfGainSlider) {
        processorRef.postLowShelfGainSliderValue = postLowShelfGainSlider.getValue();
    } else if (slider == &postHighShelfGainSlider){
        processorRef.postHighShelfGainSliderValue = postHighShelfGainSlider.getValue();
    } else if (slider == &crossSeedSlider){
        processorRef.crossSeedSliderValue = crossSeedSlider.getValue();
    }
    
    
    else if(slider == &lineCountSlider) {
        processorRef.lineCountSliderValue = lineCountSlider.getValue()/12.0;
    } else if (slider == &diffusionStagesSlider){
        processorRef.diffusionStagesSliderValue = diffusionStagesSlider.getValue()/8.0;
    } else if (slider == &lateDiffusionStagesSlider){
        processorRef.lateDiffusionStagesSliderValue = lateDiffusionStagesSlider.getValue()/8.0;
    }

    else if(slider == &tapSeedSlider) {
        processorRef.tapSeedSliderValue = tapSeedSlider.getValue() / 1000000 + 0.001;
    } else if (slider == &diffusionSeedSlider){
        processorRef.diffusionSeedSliderValue = diffusionSeedSlider.getValue() / 1000000 + 0.001;
    } else if (slider == &delaySeedSlider){
        processorRef.delaySeedSliderValue = delaySeedSlider.getValue() / 1000000 + 0.001;
    } else if (slider == &postDiffusionSeedSlider){
           processorRef.postDiffusionSeedSliderValue = postDiffusionSeedSlider.getValue() / 1000000 + 0.001;
    }

    // Note: updateParameters() is called from processBlock() on the audio thread
    // to avoid thread safety issues. The member variables are updated here on the UI thread.
}


AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.setColour (juce::Colours::darkgrey);
    g.fillRoundedRectangle(firstCol-potSize*0.0625, potSize*1.125, potSize*4.125, potSize*2, 15);
    g.fillRoundedRectangle(firstCol-potSize*0.0625, 260, potSize*4.125, 155, 15);
    g.fillRoundedRectangle(secondCol-potSize*0.0625, potSize*1.125, 335, 325, 15);
    g.fillRoundedRectangle(thirdCol-potSize*0.0625, potSize*1.125, 335, 325, 15);
    g.fillRoundedRectangle(fourthCol-potSize*0.0625, potSize*1.125, 250, 325, 15);

    g.setColour (juce::Colours::grey);
    g.fillRoundedRectangle(secondCol, 135, potSize*2, 270, 15);
    g.fillRoundedRectangle(secondCol+165, 135, potSize*2, 270, 15);
    g.fillRoundedRectangle(thirdCol, 135, potSize*2, 270, 15);
    g.fillRoundedRectangle(thirdCol+165, 135, potSize*2, 270, 15);

    
    
    g.setColour (juce::Colours::white);
    g.setFont (40.0f);    
    g.drawFittedText ("  CloudSeed", 0, 25, getWidth(), 35, juce::Justification::centred, 1);
    
    g.setFont (12.0f);
    g.setColour (juce::Colours::grey);
    g.drawFittedText ("Reverb Algorithm: Valdemar Orn", 0, 20, getWidth()-15, 35, juce::Justification::right, 1);
    g.drawFittedText ("Implementation: Steffen Sennert", 0, 37, getWidth()-15, 35, juce::Justification::right, 1);

    
    g.setFont (20.0f);
    g.setColour (juce::Colours::grey);
    g.drawFittedText ("Input Chain", firstCol+10, potSize*1.25, potSize*4, 30, juce::Justification::left, 1);
    g.drawFittedText ("Early Reflections", secondCol+10, potSize*1.25, potSize*4, 30, juce::Justification::left, 1);
    g.drawFittedText ("Late Reverbation", thirdCol+10, potSize*1.25, potSize*4, 30, juce::Justification::left, 1);
    g.drawFittedText ("Equlization", fourthCol+10, potSize*1.25, potSize*4, 30, juce::Justification::left, 1);
    g.drawFittedText ("Output Mixer", firstCol+10, thirdRowFont, potSize*4, 30, juce::Justification::left, 1);
    
    g.setColour (juce::Colours::darkgrey);
    g.drawFittedText ("Multitap Delay", secondCol+10, thirdRowFont, potSize*4, 30, juce::Justification::left, 1);
    g.drawFittedText ("Diffuser", secondCol+175, thirdRowFont, potSize*4, 30, juce::Justification::left, 1);
    g.drawFittedText ("Delay", thirdCol+10, thirdRowFont, potSize*4, 30, juce::Justification::left, 1);
    g.drawFittedText ("Diffuser", thirdCol+175, thirdRowFont, potSize*4, 30, juce::Justification::left, 1);
    
    
    g.setFont (14.0f);
    g.setColour (juce::Colours::white);

    g.drawFittedText ("Tap Seed", secondCol, thirdRowFont+potSize*0.75, potSize*2, 30, juce::Justification::centred, 1);
    g.drawFittedText ("Diffusion Seed", secondCol+potSize*2.0625, thirdRowFont+potSize*0.75, potSize*2, 30, juce::Justification::centred, 1);
    g.drawFittedText ("Delay Seed", thirdCol, thirdRowFont+potSize*0.75, potSize*2, 30, juce::Justification::centred, 1);
    g.drawFittedText ("Late Diffusion Seed", thirdCol+potSize*2.0625, thirdRowFont+potSize*0.75, potSize*2, 30, juce::Justification::centred, 1);

    g.drawFittedText ("Preset", firstCol, thirdRowFont+potSize*0.8, potSize, 20, juce::Justification::centred, 1);

    
    g.setFont (12.0f);
    g.setColour (juce::Colours::white);
    g.drawFittedText ("Lines in Parallel:", thirdCol+potSize*2+15, 100, 320, 30, juce::Justification::left, 1);
    g.drawFittedText ("Interpolation", firstCol+potSize*2+15, thirdRowFont, 320, 30, juce::Justification::left, 1);
    

    g.drawFittedText ("Input Mix", firstCol, firstRowFont, potSize, 20, juce::Justification::centred, 1);
    g.drawFittedText ("Predelay", (firstCol+potSize), firstRowFont, potSize, 20, juce::Justification::centred, 1);
    g.drawFittedText ("Low Cut", (firstCol+potSize*2), firstRowFont, potSize, 20, juce::Justification::centred, 1);
    g.drawFittedText ("High Cut", (firstCol+potSize*3), firstRowFont, potSize, 20, juce::Justification::centred, 1);
    
    g.drawFittedText ("Dry Out", firstCol, secondRowFont, potSize, 20, juce::Justification::centred, 1);
    g.drawFittedText ("Predelay Out", (firstCol+potSize), secondRowFont, potSize, 20, juce::Justification::centred, 1);
    g.drawFittedText ("Early Out", (firstCol+potSize*2), secondRowFont, potSize, 20, juce::Justification::centred, 1);
    g.drawFittedText ("Main Out", (firstCol+potSize*3), secondRowFont, potSize, 20, juce::Justification::centred, 1);
    
    g.drawFittedText ("Tap Count", secondCol, firstRowFont, potSize, 20, juce::Justification::centred, 1);
    g.drawFittedText ("Tap Length", (secondCol+potSize), firstRowFont, potSize, 20, juce::Justification::centred, 1);
    g.drawFittedText ("Delay", (secondCol+potSize*2+5), firstRowFont, potSize, 20, juce::Justification::centred, 1);
    g.drawFittedText ("Mod Amount", (secondCol+potSize*3+5), firstRowFont-potSize*0.30, potSize, 20, juce::Justification::centred, 1);
    
    g.drawFittedText ("Tap Gain", secondCol, secondRowFont, potSize, 20, juce::Justification::centred, 1);
    g.drawFittedText ("Tap Decay", (secondCol+potSize), secondRowFont, potSize, 20, juce::Justification::centred, 1);
    g.drawFittedText ("Feedback", (secondCol+potSize*2.0625), secondRowFont, potSize, 20, juce::Justification::centred, 1);
    g.drawFittedText ("Mod Rate", (secondCol+potSize*3.0625), secondRowFont-potSize*0.7, potSize, 20, juce::Justification::centred, 1);
    
    g.drawFittedText ("Stages", (secondCol+potSize*3+5), secondRowFont, potSize, 20, juce::Justification::centred, 1);


    g.drawFittedText ("Time", thirdCol, firstRowFont, potSize, 20, juce::Justification::centred, 1);
    g.drawFittedText ("Mod Amount", (thirdCol+potSize), firstRowFont-potSize*0.30, potSize, 20, juce::Justification::centred, 1);
    g.drawFittedText ("Delay", (thirdCol+potSize*2.0625), firstRowFont, potSize, 20, juce::Justification::centred, 1);
    g.drawFittedText ("Mod Amount", (thirdCol+potSize*3.0625), firstRowFont-potSize*0.30, potSize, 20, juce::Justification::centred, 1);
    
    g.drawFittedText ("Stages", (thirdCol+potSize*3+5), secondRowFont, potSize, 20, juce::Justification::centred, 1);


    g.drawFittedText ("Line Decay", thirdCol, secondRowFont, potSize, 20, juce::Justification::centred, 1);
    g.drawFittedText ("Mod Rate", (thirdCol+potSize), secondRowFont-potSize*0.7, potSize, 20, juce::Justification::centred, 1);
    g.drawFittedText ("Feedback", (thirdCol+potSize*2.0625), secondRowFont, potSize, 20, juce::Justification::centred, 1);
    g.drawFittedText ("Mod Rate", (thirdCol+potSize*3.0625), secondRowFont-potSize*0.7, potSize, 20, juce::Justification::centred, 1);
    
    
    g.drawFittedText ("Low Frequency", fourthCol, firstRowFont, potSize, 20, juce::Justification::centred, 1);
    g.drawFittedText ("High Frequency", (fourthCol+potSize), firstRowFont, potSize, 20, juce::Justification::centred, 1);
    g.drawFittedText ("Cross Seed", (fourthCol+potSize*2), firstRowFont, potSize, 20, juce::Justification::centred, 1);
    
    g.drawFittedText ("Low Gain", fourthCol, secondRowFont, potSize, 20, juce::Justification::centred, 1);
    g.drawFittedText ("High Gain", (fourthCol+potSize), secondRowFont, potSize, 20, juce::Justification::centred, 1);
    g.drawFittedText ("Cutoff", (fourthCol+potSize*2), secondRowFont, potSize, 20, juce::Justification::centred, 1);
}



void AudioPluginAudioProcessorEditor::resized()
{
    inputMixSlider.setBounds (firstCol, firstRow, potSize, potSize);
    //inputMixSlider.setBoundsRelative (firstCol, firstRow, potSize, potSize);

    predelaySlider.setBounds ( (firstCol+potSize), firstRow, potSize, potSize);
    lowCutSlider.setBounds ( (firstCol+potSize*2), firstRow, potSize, potSize);
    highCutSlider.setBounds ( (firstCol+potSize*3), firstRow, potSize, potSize);
    
    dryOutSlider.setBounds (firstCol, secondRow, potSize, potSize);
    predelayOutSlider.setBounds ( (firstCol+potSize), secondRow, potSize, potSize);
    earlyOutSlider.setBounds ( (firstCol+potSize*2), secondRow, potSize, potSize);
    mainOutSlider.setBounds ( (firstCol+potSize*3), secondRow, potSize, potSize);
     
    tapCountSlider.setBounds (secondCol, firstRow, potSize, potSize);
    tapLengthSlider.setBounds ( (secondCol+potSize), firstRow, potSize, potSize);
    diffusionDelaySlider.setBounds ( (secondCol+potSize*2.0625), firstRow, potSize, potSize);
    earlyDiffusionModAmountSlider.setBounds ( (secondCol+potSize*3.0625), firstRow, potSize, potSize*0.8);
    
    tapGainSlider.setBounds (secondCol, secondRow, potSize, potSize);
    tapDecaySlider.setBounds ( (secondCol+potSize), secondRow, potSize, potSize);
    diffusionFeedbackSlider.setBounds ( (secondCol+potSize*2.0625), secondRow, potSize, potSize);
    earlyDiffusionModRateSlider.setBounds ( (secondCol+potSize*3.0625), secondRow-potSize*0.4, potSize, potSize*0.8);
    
  
    lineDelaySlider.setBounds (thirdCol, firstRow, potSize, potSize);
    lineModAmountSlider.setBounds ( (thirdCol+potSize), firstRow, potSize, potSize*0.8);
    lateDiffusionDelaySlider.setBounds ( (thirdCol+potSize*2.0625), firstRow, potSize, potSize);
    lateDiffusionModAmountSlider.setBounds ( (thirdCol+potSize*3.0625), firstRow, potSize, potSize*0.8);
    
    lineDecaySlider.setBounds (thirdCol, secondRow, potSize, potSize);
    lineModRateSlider.setBounds ( (thirdCol+potSize), secondRow-potSize*0.4, potSize, potSize*0.8);
    lateDiffusionFeedbackSlider.setBounds ( (thirdCol+potSize*2.0625), secondRow, potSize, potSize);
    lateDiffusionModRateSlider.setBounds ( (thirdCol+potSize*3.0625), secondRow-potSize*0.4, potSize, potSize*0.8);
    
    
    postLowShelfFrequencySlider.setBounds (fourthCol, firstRow, potSize, potSize);
    postHighShelfFrequencySlider.setBounds ( (fourthCol+potSize), firstRow, potSize, potSize);
    crossSeedSlider.setBounds ( (fourthCol+potSize*2), firstRow, potSize, potSize);
    
    postLowShelfGainSlider.setBounds (fourthCol, secondRow, potSize, potSize);
    postHighShelfGainSlider.setBounds ( (fourthCol+potSize), secondRow, potSize, potSize);
    postCutoffFrequencySlider.setBounds ( (fourthCol+potSize*2), secondRow, potSize, potSize);


    diffusionStagesSlider.setBounds ( (secondCol+potSize*3+potSize*0.25), secondRow+potSize*0.7, potSize/5*3, potSize/5);
    lineCountSlider.setBounds ((thirdCol+potSize*3+potSize*0.25), 108, potSize/5*3, potSize/5);
    lateDiffusionStagesSlider.setBounds ( (thirdCol+potSize*3+potSize*0.25), secondRow+potSize*0.7, potSize/5*3, potSize/5);
    
    
    diffusionEnabledButton.setBounds(secondCol+potSize*3.3125, thirdRowFont+5, potSize*0.5, 20);
    lateDiffusionEnabledButton.setBounds(thirdCol+potSize*3.3125, thirdRowFont+5, potSize*0.5, 20);

    hiPassEnabledButton.setBounds(firstCol+potSize*2.25, firstRow-potSize*0.25, potSize*0.5, 20);
    lowPassEnabledButton.setBounds(firstCol+potSize*3.25, firstRow-potSize*0.25, potSize*0.5, 20);

    lowShelfEnabledButton.setBounds(fourthCol+potSize*0.25, thirdRowFont+5, potSize*0.5, 20);
    highShelfEnabledButton.setBounds(fourthCol+potSize*1.25, thirdRowFont+5, potSize*0.5, 20);
    cutoffEnabledButton.setBounds(fourthCol+potSize*2.25, thirdRowFont+5, potSize*0.5, 20);
    
    lateStageTapButton.setBounds(thirdCol+potSize*1.25, thirdRowFont+5, potSize*0.5, 20);
    interpolationButton.setBounds(firstCol+potSize*3.25, thirdRowFont+5, potSize*0.5, 20);
    
    
    presetSelector.setBounds (firstCol+potSize*0.125, thirdRowFont+potSize+10, potSize*2.75, 25);
    
    tapSeedSlider.setBounds (secondCol+potSize*0.25, thirdRowFont+potSize+10, potSize*1.5, 25);
    diffusionSeedSlider.setBounds (secondCol+potSize*2.25+5, thirdRowFont+potSize+10, potSize*1.5, 25);
    delaySeedSlider.setBounds (thirdCol+potSize*0.25, thirdRowFont+potSize+10, potSize*1.5, 25);
    postDiffusionSeedSlider.setBounds (thirdCol+potSize*2.25+5, thirdRowFont+potSize+10, potSize*1.5, 25);
}





