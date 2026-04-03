#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "audio_engine/ReverbController.h"

//==============================================================================
class AudioPluginAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    AudioPluginAudioProcessor();
    ~AudioPluginAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    //==============================================================================
    
    
    //void AudioProcessorParameter::setValueNotifyHost(float);
//    void loadPresetProgram (int index);

    
    void updateParameters();
    void syncParametersFromTreeState();  // Sync manual variables from treeState (for state loading/automation)

    int currentProgramNumber = {0};
    
    float inputMixSliderValue;
    float predelaySliderValue;
    float lowCutSliderValue;
    float highCutSliderValue;
    
    float dryOutSliderValue;
    float predelayOutSliderValue;
    float earlyOutSliderValue;
    float mainOutSliderValue;
    
    
    float tapCountSliderValue;
    float tapLengthSliderValue;
    float diffusionDelaySliderValue;
    float earlyDiffusionModAmountSliderValue;
    
    float tapGainSliderValue;
    float tapDecaySliderValue;
    float diffusionFeedbackSliderValue;
    float earlyDiffusionModRateSliderValue;
    
    
    float lineDelaySliderValue;
    float lineModAmountSliderValue;
    float lateDiffusionDelaySliderValue;
    float lateDiffusionModAmountSliderValue;
    
    float lineDecaySliderValue;
    float lineModRateSliderValue;
    float lateDiffusionFeedbackSliderValue;
    float lateDiffusionModRateSliderValue;
    
    
    float postLowShelfFrequencySliderValue;
    float postHighShelfFrequencySliderValue;
    float postCutoffFrequencySliderValue;
    
    float postLowShelfGainSliderValue;
    float postHighShelfGainSliderValue;
    float crossSeedSliderValue;
    
    
    float tapSeedSliderValue;
    float diffusionSeedSliderValue;
    float delaySeedSliderValue;
    float postDiffusionSeedSliderValue;
    

    float lineCountSliderValue;
    float diffusionStagesSliderValue;
    float lateDiffusionStagesSliderValue;
    
    float diffusionEnabledButtonValue = {1.0f};
    float lateDiffusionEnabledButtonValue = {1.0f};;

    float hiPassEnabledButtonValue = {0.0f};
    float lowPassEnabledButtonValue = {0.0f};
    float cutoffEnabledButtonValue = {1.0f};
    
    float lowShelfEnabledButtonValue = {0.0f};
    float highShelfEnabledButtonValue = {0.0f};
    
    float lateStageTapButtonValue = {1.0f};
    float interpolationButtonValue = {0.0f};
    
    void initChorusPreset();
//    void initFactoryDullEchos();
//    void initFactoryHyperplane();
//    void initFactoryMediumSpace();
//    void initFactoryNoiseInTheHallway();
//    void initFactoryRubiKaFields();
//    void initFactorySmallRoom();
//    void initFactory90sAreBack();
//    void initFactoryThroughTheLookingGlass();
    
    
    juce::AudioProcessorValueTreeState treeState;
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    
private:
    CloudSeed::ReverbController reverb;
    
        
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessor)
};


