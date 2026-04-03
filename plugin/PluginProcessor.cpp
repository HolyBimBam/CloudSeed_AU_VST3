#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "audio_engine/FastSin.h"
#include "audio_engine/AudioLib/ValueTables.h"

//==============================================================================
AudioPluginAudioProcessor::AudioPluginAudioProcessor()
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
                        treeState(*this,nullptr, "PARAMETERS", createParameterLayout()),
                        reverb(48000)  // Default to 48kHz, will be updated in prepareToPlay
{

    AudioLib::ValueTables::Init();
    CloudSeed::FastSin::Init();
    reverb.ClearBuffers(); // clear buffers before we start do dsp stuff.
  
    initChorusPreset();
    updateParameters();
//    loadPresetProgram(0);
    
    
}



AudioPluginAudioProcessor::~AudioPluginAudioProcessor()
{
}

//==============================================================================


juce::AudioProcessorValueTreeState::ParameterLayout AudioPluginAudioProcessor::createParameterLayout()
{
    std::vector <std::unique_ptr<juce::RangedAudioParameter>> params;
    
    
    auto inputMixParam = std::make_unique<juce::AudioParameterFloat>("inputMix", "InputMix", 0.0f, 1.0f, 0.0f);
    params.push_back(std::move(inputMixParam));
    auto predelayParam = std::make_unique<juce::AudioParameterFloat>("predelay", "Predelay", 0.0f, 1000.0f, 70.0f);
    params.push_back(std::move(predelayParam));
    auto lowCutParam = std::make_unique<juce::AudioParameterFloat>("lowCut", "LowCut", 0.0f, 1.0f, 0.29000008106231689f);
    params.push_back(std::move(lowCutParam));
    auto highCutParam = std::make_unique<juce::AudioParameterFloat>("highCut", "HighCut", 0.0f, 1.0f, 0.0f);
    params.push_back(std::move(highCutParam));

    auto dryOutParam = std::make_unique<juce::AudioParameterFloat>("dryOut", "DryOut", 0.0f, 1.0f, 0.94499987363815308f);
    params.push_back(std::move(dryOutParam));
    auto predelayOutParam = std::make_unique<juce::AudioParameterFloat>("predelayOut", "PredelayOut", 0.0f, 1.0f, 0.0f);
    params.push_back(std::move(predelayOutParam));
    auto earlyOutParam = std::make_unique<juce::AudioParameterFloat>("earlyOut", "EarlyOut", 0.0f, 1.0f, 0.77999997138977051f);
    params.push_back(std::move(earlyOutParam));
    auto mainOutParam = std::make_unique<juce::AudioParameterFloat>("mainOut", "MainOut", 0.0f, 1.0f, 0.74500006437301636f);
    params.push_back(std::move(mainOutParam));

    
    auto tapCountParam = std::make_unique<juce::AudioParameterFloat>("tapCount", "TapCount", 0.0f, 1.0f, 0.36499997973442078f);
    params.push_back(std::move(tapCountParam));
    auto tapLengthParam = std::make_unique<juce::AudioParameterFloat>("tapLength", "TapLength", 0.0f, 500.0f, 500.0f);
    params.push_back(std::move(tapLengthParam));
    auto diffusionDelayParam = std::make_unique<juce::AudioParameterFloat>("diffusionDelay", "DiffusionDelay", 0.0f, 1.0f, 0.43500006198883057f);
    params.push_back(std::move(diffusionDelayParam));
    auto earlyDiffusionModAmountParam = std::make_unique<juce::AudioParameterFloat>("earlyDiffusionModAmount", "EarlyDiffusionModAmount", 0.0f, 2.5f, (0.50000005960464478f*2.5));
    params.push_back(std::move(earlyDiffusionModAmountParam));
    
    auto tapGainParam = std::make_unique<juce::AudioParameterFloat>("tapGain", "TapGain", 0.0f, 1.0f, 1.0f);
    params.push_back(std::move(tapGainParam));
    auto tapDecayParam = std::make_unique<juce::AudioParameterFloat>("tapDecay", "TapDecay", 0.0f, 1.0f, 0.86500012874603271f);
    params.push_back(std::move(tapDecayParam));
    auto diffusionFeedbackParam = std::make_unique<juce::AudioParameterFloat>("diffusionFeedback", "DiffusionFeedback", 0.0f, 1.0f, 0.725000262260437f);
    params.push_back(std::move(diffusionFeedbackParam));
    auto earlyDiffusionModRateParam = std::make_unique<juce::AudioParameterFloat>("earlyDiffusionModRate", "EarlyDiffusionModRate", 0.0f, 1.0f, 0.42500010132789612f);
    params.push_back(std::move(earlyDiffusionModRateParam));

    
    auto lineDelayParam = std::make_unique<juce::AudioParameterFloat>("lineDelay", "LineDelay", 0.0f, 1.0f, 0.68499988317489624f);
    params.push_back(std::move(lineDelayParam));
    auto lineModAmountParam = std::make_unique<juce::AudioParameterFloat>("lineModAmount", "LineModAmount", 0.0f, 2.5f, (0.59000003337860107f*2.5));
    params.push_back(std::move(lineModAmountParam));
    auto lateDiffusionDelaySliderParam = std::make_unique<juce::AudioParameterFloat>("lateDiffusionDelay", "LateDiffusionDelay", 0.0f, 1.0f, 0.54499995708465576f);
    params.push_back(std::move(lateDiffusionDelaySliderParam));
    auto lateDiffusionModAmountParam = std::make_unique<juce::AudioParameterFloat>("lateDiffusionModAmount", "LateDiffusionModAmount", 0.0f, 2.5f, (0.619999885559082f*2.5));
    params.push_back(std::move(lateDiffusionModAmountParam));
    
    auto lineDecayParam = std::make_unique<juce::AudioParameterFloat>("lineDecay", "LineDecay", 0.0f, 1.0f, 0.68000012636184692f);
    params.push_back(std::move(lineDecayParam));
    auto lineModRateParam = std::make_unique<juce::AudioParameterFloat>("lineModRate", "LineModRate", 0.0f, 1.0f, 0.46999993920326233f);
    params.push_back(std::move(lineModRateParam));
    auto lateDiffusionFeedbackParam = std::make_unique<juce::AudioParameterFloat>("lateDiffusionFeedback", "LateDiffusionFeedback", 0.0f, 1.0f, 0.65999996662139893f);
    params.push_back(std::move(lateDiffusionFeedbackParam));
    auto lateDiffusionModRateParam = std::make_unique<juce::AudioParameterFloat>("lateDiffusionModRate", "LateDiffusionModRate", 0.0f, 1.0f, 0.42500019073486328f);
    params.push_back(std::move(lateDiffusionModRateParam));
    
    
    auto postLowShelfFrequencyParam = std::make_unique<juce::AudioParameterFloat>("postLowShelfFrequency", "PostLowShelfFrequency", 0.0f, 1.0f, 0.31499990820884705f);
    params.push_back(std::move(postLowShelfFrequencyParam));
    auto postHighShelfFrequencyParam = std::make_unique<juce::AudioParameterFloat>("postHighShelfFrequency", "PostHighShelfFrequency", 0.0f, 1.0f, 0.73000013828277588f);
    params.push_back(std::move(postHighShelfFrequencyParam));
    auto postCutoffFrequencyParam = std::make_unique<juce::AudioParameterFloat>("postCutoffFrequency", "PostCutoffFrequency", 0.0f, 1.0f, 0.73499983549118042f);
    params.push_back(std::move(postCutoffFrequencyParam));

    auto postLowShelfGainParam = std::make_unique<juce::AudioParameterFloat>("postLowShelfGain", "PostLowShelfGain", 0.0f, 1.0f, 0.5199999213218689f);
    params.push_back(std::move(postLowShelfGainParam));
    auto postHighShelfGainParam = std::make_unique<juce::AudioParameterFloat>("postHighShelfGain", "PostHighShelfGain", 0.0f, 1.0f, 0.83500003814697266f);
    params.push_back(std::move(postHighShelfGainParam));
    auto crossSeedParam = std::make_unique<juce::AudioParameterFloat>("crossSeed", "CrossSeed", 0.0f, 1.0f, 0.0f);
    params.push_back(std::move(crossSeedParam));

    
     
    auto tapSeedParam = std::make_unique<juce::AudioParameterInt>("tapSeed", "TapSeed", 0, 1000000, ((int)(0.0011500000255182385f * 1000000 + 0.001)));
    params.push_back(std::move(tapSeedParam));
    auto diffusionSeedParam = std::make_unique<juce::AudioParameterInt>("diffusionSeed", "DiffusionSeed", 0, 1000000, ((int)(0.00018899999849963933f * 1000000 + 0.001)));
    params.push_back(std::move(diffusionSeedParam));
    auto delaySeedParam = std::make_unique<juce::AudioParameterInt>("delaySeed", "DelaySeed", 0, 1000000, ((int)(0.00034699999378062785f * 1000000 + 0.001)));
    params.push_back(std::move(delaySeedParam));
    auto postDiffusionSeedParam = std::make_unique<juce::AudioParameterInt>("postDiffusionSeed", "PostDiffusionSeed", 0, 1000000, ((int)(0.00050099997315555811f * 1000000 + 0.001)));
    params.push_back(std::move(postDiffusionSeedParam));
    
    

    
    auto lineCountParam = std::make_unique<juce::AudioParameterInt>("lineCount", "LineCount", 1, 12, 12);
    params.push_back(std::move(lineCountParam));
    auto diffusionStagesParam = std::make_unique<juce::AudioParameterInt>("diffusionStages", "DiffusionStages", 1, 8, 4);
    params.push_back(std::move(diffusionStagesParam));
    auto lateDiffusionStagesParam = std::make_unique<juce::AudioParameterInt>("lateDiffusionStages", "LateDiffusionStages", 1, 8, 3);
    params.push_back(std::move(lateDiffusionStagesParam));

    auto diffusionEnabledParam = std::make_unique<juce::AudioParameterFloat>("diffusionEnabled", "DiffusionEnabled", 0.0f, 1.0f, 1.0f);
    params.push_back(std::move(diffusionEnabledParam));
    auto lateDiffusionEnabledParam = std::make_unique<juce::AudioParameterFloat>("lateDiffusionEnabled", "LateDiffusionEnabled", 0.0f, 1.0f, 1.0f);
    params.push_back(std::move(lateDiffusionEnabledParam));
  
    auto highPassEnabledParam = std::make_unique<juce::AudioParameterFloat>("highPassEnabled", "HighPassEnabled", 0.0f, 1.0f, 0.0f);
    params.push_back(std::move(highPassEnabledParam));
    auto lowPassEnabledParam = std::make_unique<juce::AudioParameterFloat>("lowPassEnabled", "LowPassEnabled", 0.0f, 1.0f, 0.0f);
    params.push_back(std::move(lowPassEnabledParam));
    
    auto lowShelfEnabledParam = std::make_unique<juce::AudioParameterFloat>("lowShelfEnabled", "LowShelfEnabled", 0.0f, 1.0f, 0.0f);
    params.push_back(std::move(lowShelfEnabledParam));
    auto highShelfEnabledParam = std::make_unique<juce::AudioParameterFloat>("highShelfEnabled", "HighShelfEnabled", 0.0f, 1.0f, 0.0f);
    params.push_back(std::move(highShelfEnabledParam));

    auto cutoffEnabledParam = std::make_unique<juce::AudioParameterFloat>("cutoffEnabled", "CutoffEnabled", 0.0f, 1.0f, 1.0f);
    params.push_back(std::move(cutoffEnabledParam));
    
    auto interpolationParam = std::make_unique<juce::AudioParameterFloat>("interpolation", "Interpolation", 0.0f, 1.0f, 0.0f);
    params.push_back(std::move(interpolationParam));
    
    auto lateStageTapParam = std::make_unique<juce::AudioParameterFloat>("lateStageTap", "LateStageTap", 0.0f, 1.0f, 0.0f);
    params.push_back(std::move(lateStageTapParam));
    
    
    
    return{ params.begin(), params.end() };
}


void AudioPluginAudioProcessor::initChorusPreset()
{
       inputMixSliderValue = {0.0f};
//    treeState.setValueNotifyHost();
       predelaySliderValue = {(0.070000000298023224*1000.0)};
       lowCutSliderValue = {0.0f};
       highCutSliderValue = {0.29000008106231689};
       //treeState. sendValueChangedMessageToListeners

       dryOutSliderValue = {0.94499987363815308f};
       predelayOutSliderValue = {0.0f};
       earlyOutSliderValue = {0.77999997138977051f};
       mainOutSliderValue = {0.74500006437301636f};

       tapCountSliderValue = {0.36499997973442078};
       tapLengthSliderValue = {1.0f};
       diffusionDelaySliderValue = {0.43500006198883057};
       earlyDiffusionModAmountSliderValue = {(0.50000005960464478*2.5)};

       tapGainSliderValue = {1.0f};
       tapDecaySliderValue = {0.86500012874603271};
       diffusionFeedbackSliderValue = {0.725000262260437};
       earlyDiffusionModRateSliderValue = {0.42500010132789612};


       lineDelaySliderValue = {0.68499988317489624f};
       lineModAmountSliderValue = {(0.59000003337860107f*2.5)};
       lateDiffusionDelaySliderValue = {0.68499988317489624f};
       lateDiffusionModAmountSliderValue = {(0.619999885559082f*2.5)};

       lineDecaySliderValue = {0.68000012636184692f};
       lineModRateSliderValue = {0.46999993920326233f};
       lateDiffusionFeedbackSliderValue = {0.65999996662139893f};
       lateDiffusionModRateSliderValue = {0.42500019073486328f};


       postLowShelfFrequencySliderValue = {0.31499990820884705f};
       postHighShelfFrequencySliderValue = {0.73000013828277588f};
       postCutoffFrequencySliderValue = {0.73499983549118042f};

       postLowShelfGainSliderValue = {0.5199999213218689f};
       postHighShelfGainSliderValue = {0.83500003814697266f};
       crossSeedSliderValue = {0.0f};


       tapSeedSliderValue = {(int)(0.0011500000255182385*1000000+0.001)};
       diffusionSeedSliderValue = {(int)(0.00018899999849963933*1000000+0.001)};
       delaySeedSliderValue = {(int)(0.00033700000494718552*1000000+0.001)};
       postDiffusionSeedSliderValue = {(int)(0.00050099997315555811*1000000+0.001)};


       lineCountSliderValue = {1.0f};
       diffusionStagesSliderValue = {0.4285714328289032f};
       lateDiffusionStagesSliderValue = {0.28571429848670959f};

       diffusionEnabledButtonValue = {1.0f};
       lateDiffusionEnabledButtonValue = {1.0f};;

       hiPassEnabledButtonValue = {0.0f};
       lowPassEnabledButtonValue = {0.0f};
       cutoffEnabledButtonValue = {1.0f};

       lowShelfEnabledButtonValue = {0.0f};
       highShelfEnabledButtonValue = {0.0f};

       lateStageTapButtonValue = {1.0f};
       interpolationButtonValue = {0.0f};

}
//
//
//void AudioPluginAudioProcessor::initFactoryDullEchos()
//{
//                //parameters from Dull Echos in
//                //https://github.com/ValdemarOrn/CloudSeed/tree/master/Factory%20Programs
//                inputMixSliderValue = 0.0;
//                predelaySliderValue = 0.070000000298023224;
//                lowCutSliderValue = 0.0;
//                highCutSliderValue = 0.29000008106231689;
//                tapCountSliderValue = 0.36499997973442078;
//                tapLengthSliderValue = 1.0;
//                tapGainSliderValue = 0.83499991893768311;
//                tapDecaySliderValue = 0.86500012874603271;
//                diffusionEnabledButtonValue = 1.0;
//                diffusionStagesSliderValue = 0.4285714328289032;
//                diffusionDelaySliderValue = 0.43500006198883057;
//                diffusionFeedbackSliderValue = 0.725000262260437;
//                lineCountSliderValue = 1.0;
//                lineDelaySliderValue = 0.34500002861022949;
//                lineDecaySliderValue = 0.41500008106231689;
//                lateDiffusionEnabledButtonValue = 0.0;
//                lateDiffusionStagesSliderValue = 0.57142859697341919;
//                lateDiffusionDelaySliderValue = 0.66499996185302734;
//                lateDiffusionFeedbackSliderValue = 0.61000001430511475;
//                postLowShelfGainSliderValue = 0.5199999213218689;
//                postLowShelfFrequencySliderValue = 0.31499990820884705;
//                postHighShelfGainSliderValue = 0.83500003814697266;
//                postHighShelfFrequencySliderValue = 0.73000013828277588;
//                postCutoffFrequencySliderValue = 0.73499983549118042;
//                earlyDiffusionModAmountSliderValue = 0.25499999523162842;
//                earlyDiffusionModRateSliderValue = 0.3250001072883606;
//                lineModAmountSliderValue = 0.33500000834465027;
//                lineModRateSliderValue = 0.26999998092651367;
//                lateDiffusionModAmountSliderValue = 0.13499975204467773;
//                lateDiffusionModRateSliderValue = 0.27500006556510925;
//                tapSeedSliderValue = 0.0011500000255182385;
//                diffusionSeedSliderValue = 0.00018899999849963933;
//                delaySeedSliderValue = 0.0002730000123847276;
//                postDiffusionSeedSliderValue = 0.00050099997315555811;
//                crossSeedSliderValue = 0.5;
//                dryOutSliderValue = 1.0;
//                predelayOutSliderValue = 0.0;
//                earlyOutSliderValue = 0.77999997138977051;
//                mainOutSliderValue = 0.74500006437301636;
//                hiPassEnabledButtonValue = 0.0;
//                lowPassEnabledButtonValue = 1.0;
//                lowShelfEnabledButtonValue = 0.0;
//                highShelfEnabledButtonValue = 0.0;
//                cutoffEnabledButtonValue = 1.0;
//                lateStageTapButtonValue = 0.0;
//                interpolationButtonValue = 1.0;
//
//
//
//}
//
//
//            void AudioPluginAudioProcessor::initFactoryHyperplane()
//            {
//                //parameters from Hyperplane in
//                //https://github.com/ValdemarOrn/CloudSeed/tree/master/Factory%20Programs
//                inputMixSliderValue = 0.1549999862909317;
//                predelaySliderValue = 0.0;
//                lowCutSliderValue = 0.57999998331069946;
//                highCutSliderValue = 0.9100000262260437;
//                tapCountSliderValue = 0.41499990224838257;
//                tapLengthSliderValue = 0.43999996781349182;
//                tapGainSliderValue = 1.0;
//                tapDecaySliderValue = 1.0;
//                diffusionEnabledButtonValue = 1.0;
//                diffusionStagesSliderValue = 0.4285714328289032;
//                diffusionDelaySliderValue = 0.27500024437904358;
//                diffusionFeedbackSliderValue = 0.660000205039978;
//                lineCountSliderValue = 0.72727274894714355;
//                lineDelaySliderValue = 0.22500017285346985;
//                lineDecaySliderValue = 0.794999897480011;
//                lateDiffusionEnabledButtonValue = 1.0;
//                lateDiffusionStagesSliderValue = 1.0;
//                lateDiffusionDelaySliderValue = 0.22999951243400574;
//                lateDiffusionFeedbackSliderValue = 0.59499990940093994;
//                postLowShelfGainSliderValue = 0.95999979972839355;
//                postLowShelfFrequencySliderValue = 0.23999994993209839;
//                postHighShelfGainSliderValue = 0.97500002384185791;
//                postHighShelfFrequencySliderValue = 0.78499996662139893;
//                postCutoffFrequencySliderValue = 0.87999981641769409;
//                earlyDiffusionModAmountSliderValue = 0.13499999046325684;
//                earlyDiffusionModRateSliderValue = 0.29000008106231689;
//                lineModAmountSliderValue = 0.53999996185302734;
//                lineModRateSliderValue = 0.44999989867210388;
//                lateDiffusionModAmountSliderValue = 0.15999998152256012;
//                lateDiffusionModRateSliderValue = 0.56000012159347534;
//                tapSeedSliderValue = 0.00048499999684281647;
//                diffusionSeedSliderValue = 0.00020799999765586108;
//                delaySeedSliderValue = 0.00034699999378062785;
//                postDiffusionSeedSliderValue = 0.00037200000951997936;
//                crossSeedSliderValue = 0.800000011920929;
//                dryOutSliderValue = 0.86500018835067749;
//                predelayOutSliderValue = 0.0;
//                earlyOutSliderValue = 0.8200000524520874;
//                mainOutSliderValue = 0.79500007629394531;
//                hiPassEnabledButtonValue = 1.0;
//                lowPassEnabledButtonValue = 1.0;
//                lowShelfEnabledButtonValue = 1.0;
//                highShelfEnabledButtonValue = 1.0;
//                cutoffEnabledButtonValue = 1.0;
//                lateStageTapButtonValue = 1.0;
//                interpolationButtonValue = 0.0;
//
//
//            }
//
//            void AudioPluginAudioProcessor::initFactoryMediumSpace()
//            {
//                //parameters from Medium Space in
//                //https://github.com/ValdemarOrn/CloudSeed/tree/master/Factory%20Programs
//                inputMixSliderValue = 0.0;
//                predelaySliderValue = 0.0;
//                lowCutSliderValue = 0.0;
//                highCutSliderValue = 0.63999992609024048;
//                tapCountSliderValue = 0.51999980211257935;
//                tapLengthSliderValue = 0.26499992609024048;
//                tapGainSliderValue = 0.69499999284744263;
//                tapDecaySliderValue = 1.0;
//                diffusionEnabledButtonValue = 1.0;
//                diffusionStagesSliderValue = 0.8571428656578064;
//                diffusionDelaySliderValue = 0.5700000524520874;
//                diffusionFeedbackSliderValue = 0.76000010967254639;
//                lineCountSliderValue = 0.18181818723678589;
//                lineDelaySliderValue = 0.585000216960907;
//                lineDecaySliderValue = 0.29499980807304382;
//                lateDiffusionEnabledButtonValue = 1.0;
//                lateDiffusionStagesSliderValue = 0.57142859697341919;
//                lateDiffusionDelaySliderValue = 0.69499951601028442;
//                lateDiffusionFeedbackSliderValue = 0.71499985456466675;
//                postLowShelfGainSliderValue = 0.87999987602233887;
//                postLowShelfFrequencySliderValue = 0.19499993324279785;
//                postHighShelfGainSliderValue = 0.72000008821487427;
//                postHighShelfFrequencySliderValue = 0.520000159740448;
//                postCutoffFrequencySliderValue = 0.79999983310699463;
//                earlyDiffusionModAmountSliderValue = 0.13499999046325684;
//                earlyDiffusionModRateSliderValue = 0.26000010967254639;
//                lineModAmountSliderValue = 0.054999928921461105;
//                lineModRateSliderValue = 0.21499986946582794;
//                lateDiffusionModAmountSliderValue = 0.17999963462352753;
//                lateDiffusionModRateSliderValue = 0.38000011444091797;
//                tapSeedSliderValue = 0.0003009999927598983;
//                diffusionSeedSliderValue = 0.00018899999849963933;
//                delaySeedSliderValue = 0.0001610000035725534;
//                postDiffusionSeedSliderValue = 0.00050099997315555811;
//                crossSeedSliderValue = 0.7850000262260437;
//                dryOutSliderValue = 1.0;
//                predelayOutSliderValue = 0.0;
//                earlyOutSliderValue = 0.699999988079071;
//                mainOutSliderValue = 0.84499984979629517;
//                hiPassEnabledButtonValue = 0.0;
//                lowPassEnabledButtonValue = 1.0;
//                lowShelfEnabledButtonValue = 1.0;
//                highShelfEnabledButtonValue = 0.0;
//                cutoffEnabledButtonValue = 1.0;
//                lateStageTapButtonValue = 1.0;
//                interpolationButtonValue = 1.0;
//
//
//            }
//
//            void AudioPluginAudioProcessor::initFactoryNoiseInTheHallway()
//            {
//                //parameters from Noise In The Hallway in
//                //https://github.com/ValdemarOrn/CloudSeed/tree/master/Factory%20Programs
//                inputMixSliderValue = 0.0;
//                predelaySliderValue = 0.0;
//                lowCutSliderValue = 0.0;
//                highCutSliderValue = 0.60999995470047;
//                tapCountSliderValue = 1.0;
//                tapLengthSliderValue = 1.0;
//                tapGainSliderValue = 0.0;
//                tapDecaySliderValue = 0.830000102519989;
//                diffusionEnabledButtonValue = 1.0;
//                diffusionStagesSliderValue = 0.28571429848670959;
//                diffusionDelaySliderValue = 0.35499998927116394;
//                diffusionFeedbackSliderValue = 0.62500005960464478;
//                lineCountSliderValue = 0.63636362552642822;
//                lineDelaySliderValue = 0.36000004410743713;
//                lineDecaySliderValue = 0.51000005006790161;
//                lateDiffusionEnabledButtonValue = 1.0;
//                lateDiffusionStagesSliderValue = 0.0;
//                lateDiffusionDelaySliderValue = 0.62999987602233887;
//                lateDiffusionFeedbackSliderValue = 0.49000000953674316;
//                postLowShelfGainSliderValue = 0.0;
//                postLowShelfFrequencySliderValue = 0.0;
//                postHighShelfGainSliderValue = 0.77499985694885254;
//                postHighShelfFrequencySliderValue = 0.58000004291534424;
//                postCutoffFrequencySliderValue = 0.0;
//                earlyDiffusionModAmountSliderValue = 0.0;
//                earlyDiffusionModRateSliderValue = 0.0;
//                lineModAmountSliderValue = 0.0;
//                lineModRateSliderValue = 0.0;
//                lateDiffusionModAmountSliderValue = 0.0;
//                lateDiffusionModRateSliderValue = 0.0;
//                tapSeedSliderValue = 0.0001140000022132881;
//                diffusionSeedSliderValue = 0.000155999994603917;
//                delaySeedSliderValue = 0.00018099999579135329;
//                postDiffusionSeedSliderValue = 8.4999999671708792E-05;
//                crossSeedSliderValue = 1.0;
//                dryOutSliderValue = 0.0;
//                predelayOutSliderValue = 0.0;
//                earlyOutSliderValue = 0.64500010013580322;
//                mainOutSliderValue = 0.63000005483627319;
//                hiPassEnabledButtonValue = 0.0;
//                lowPassEnabledButtonValue = 1.0;
//                lowShelfEnabledButtonValue = 0.0;
//                highShelfEnabledButtonValue = 1.0;
//                cutoffEnabledButtonValue = 0.0;
//                lateStageTapButtonValue = 0.0;
//                interpolationButtonValue = 1.0;
//
//
//            }
//
//            void AudioPluginAudioProcessor::initFactoryRubiKaFields()
//            {
//                //parameters from Rubi-Ka Fields in
//                //https://github.com/ValdemarOrn/CloudSeed/tree/master/Factory%20Programs
//                inputMixSliderValue = 0.32499998807907104;
//                predelaySliderValue = 0.0;
//                lowCutSliderValue = 0.0;
//                highCutSliderValue = 0.8899998664855957;
//                tapCountSliderValue = 0.51999980211257935;
//                tapLengthSliderValue = 1.0;
//                tapGainSliderValue = 0.90000003576278687;
//                tapDecaySliderValue = 1.0;
//                diffusionEnabledButtonValue = 1.0;
//                diffusionStagesSliderValue = 0.8571428656578064;
//                diffusionDelaySliderValue = 0.5700000524520874;
//                diffusionFeedbackSliderValue = 0.76000010967254639;
//                lineCountSliderValue = 0.27272728085517883;
//                lineDelaySliderValue = 0.68500018119812012;
//                lineDecaySliderValue = 0.82999974489212036;
//                lateDiffusionEnabledButtonValue = 1.0;
//                lateDiffusionStagesSliderValue = 0.71428573131561279;
//                lateDiffusionDelaySliderValue = 0.69499951601028442;
//                lateDiffusionFeedbackSliderValue = 0.71499985456466675;
//                postLowShelfGainSliderValue = 0.87999987602233887;
//                postLowShelfFrequencySliderValue = 0.19499993324279785;
//                postHighShelfGainSliderValue = 0.72000008821487427;
//                postHighShelfFrequencySliderValue = 0.520000159740448;
//                postCutoffFrequencySliderValue = 0.79999983310699463;
//                earlyDiffusionModAmountSliderValue = 0.13499999046325684;
//                earlyDiffusionModRateSliderValue = 0.26000010967254639;
//                lineModAmountSliderValue = 0.054999928921461105;
//                lineModRateSliderValue = 0.21499986946582794;
//                lateDiffusionModAmountSliderValue = 0.32499963045120239;
//                lateDiffusionModRateSliderValue = 0.35500010848045349;
//                tapSeedSliderValue = 0.0003009999927598983;
//                diffusionSeedSliderValue = 0.00018899999849963933;
//                delaySeedSliderValue = 0.0001610000035725534;
//                postDiffusionSeedSliderValue = 0.00050099997315555811;
//                crossSeedSliderValue = 0.43000003695487976;
//                dryOutSliderValue = 0.88499999046325684;
//                predelayOutSliderValue = 0.0;
//                earlyOutSliderValue = 0.0;
//                mainOutSliderValue = 0.90999990701675415;
//                hiPassEnabledButtonValue = 0.0;
//                lowPassEnabledButtonValue = 0.0;
//                lowShelfEnabledButtonValue = 0.0;
//                highShelfEnabledButtonValue = 0.0;
//                cutoffEnabledButtonValue = 1.0;
//                lateStageTapButtonValue = 1.0;
//                interpolationButtonValue = 0.0;
//
//
//            }
//
//
//            void AudioPluginAudioProcessor::initFactorySmallRoom()
//            {
//                //parameters from Small Room in
//                //https://github.com/ValdemarOrn/CloudSeed/tree/master/Factory%20Programs
//                inputMixSliderValue = 0.0;
//                predelaySliderValue = 0.0;
//                lowCutSliderValue = 0.0;
//                highCutSliderValue = 0.755000114440918;
//                tapCountSliderValue = 0.41499990224838257;
//                tapLengthSliderValue = 0.43999996781349182;
//                tapGainSliderValue = 0.87999999523162842;
//                tapDecaySliderValue = 1.0;
//                diffusionEnabledButtonValue = 1.0;
//                diffusionStagesSliderValue = 0.71428573131561279;
//                diffusionDelaySliderValue = 0.335000216960907;
//                diffusionFeedbackSliderValue = 0.660000205039978;
//                lineCountSliderValue = 0.18181818723678589;
//                lineDelaySliderValue = 0.51000016927719116;
//                lineDecaySliderValue = 0.29999998211860657;
//                lateDiffusionEnabledButtonValue = 1.0;
//                lateDiffusionStagesSliderValue = 0.4285714328289032;
//                lateDiffusionDelaySliderValue = 0.22999951243400574;
//                lateDiffusionFeedbackSliderValue = 0.59499990940093994;
//                postLowShelfGainSliderValue = 0.87999987602233887;
//                postLowShelfFrequencySliderValue = 0.19499993324279785;
//                postHighShelfGainSliderValue = 0.875;
//                postHighShelfFrequencySliderValue = 0.59000009298324585;
//                postCutoffFrequencySliderValue = 0.79999983310699463;
//                earlyDiffusionModAmountSliderValue = 0.13499999046325684;
//                earlyDiffusionModRateSliderValue = 0.29000008106231689;
//                lineModAmountSliderValue = 0.18999995291233063;
//                lineModRateSliderValue = 0.22999987006187439;
//                lateDiffusionModAmountSliderValue = 0.1249999925494194;
//                lateDiffusionModRateSliderValue = 0.28500008583068848;
//                tapSeedSliderValue = 0.00048499999684281647;
//                diffusionSeedSliderValue = 0.00020799999765586108;
//                delaySeedSliderValue = 0.00033499998971819878;
//                postDiffusionSeedSliderValue = 0.00037200000951997936;
//                crossSeedSliderValue = 0.42500001192092896;
//                dryOutSliderValue = 1.0;
//                predelayOutSliderValue = 0.0;
//                earlyOutSliderValue = 0.8599998950958252;
//                mainOutSliderValue = 0.90500003099441528;
//                hiPassEnabledButtonValue = 0.0;
//                lowPassEnabledButtonValue = 1.0;
//                lowShelfEnabledButtonValue = 0.0;
//                highShelfEnabledButtonValue = 0.0;
//                cutoffEnabledButtonValue = 0.0;
//                lateStageTapButtonValue = 1.0;
//                interpolationButtonValue = 1.0;
//
//
//            }
//
//            void AudioPluginAudioProcessor::initFactory90sAreBack()
//            {
//                //parameters from The 90s Are Back in
//                //https://github.com/ValdemarOrn/CloudSeed/tree/master/Factory%20Programs
//                inputMixSliderValue = 0;
//                predelaySliderValue = 0;
//                lowCutSliderValue = 0;
//                highCutSliderValue = 0.6750001311302185;
//                tapCountSliderValue = 0;
//                tapLengthSliderValue = 1;
//                tapGainSliderValue = 0;
//                tapDecaySliderValue = 0.8650001287460327;
//                diffusionEnabledButtonValue = 1;
//                diffusionStagesSliderValue = 0.5714285969734192;
//                diffusionDelaySliderValue = 0.7100000381469727;
//                diffusionFeedbackSliderValue = 0.5450003147125244;
//                lineCountSliderValue = 0.7272727489471436;
//                lineDelaySliderValue = 0.6849998831748962;
//                lineDecaySliderValue = 0.6300000548362732;
//                lateDiffusionEnabledButtonValue = 0;
//                lateDiffusionStagesSliderValue = 0.2857142984867096;
//                lateDiffusionDelaySliderValue = 0.5449999570846558;
//                lateDiffusionFeedbackSliderValue = 0.6599999666213989;
//                postLowShelfGainSliderValue = 0.5199999213218689;
//                postLowShelfFrequencySliderValue = 0.31499990820884705;
//                postHighShelfGainSliderValue = 0.8349999189376831;
//                postHighShelfFrequencySliderValue = 0.705000102519989;
//                postCutoffFrequencySliderValue = 0.7349998354911804;
//                earlyDiffusionModAmountSliderValue = 0.824999988079071;
//                earlyDiffusionModRateSliderValue = 0.4050004780292511;
//                lineModAmountSliderValue = 0.6300000548362732;
//                lineModRateSliderValue = 0.3199999928474426;
//                lateDiffusionModAmountSliderValue = 0.619999885559082;
//                lateDiffusionModRateSliderValue = 0.30000022053718567;
//                tapSeedSliderValue = 0.0011500000255182385;
//                diffusionSeedSliderValue = 0.00018899999849963933;
//                delaySeedSliderValue = 0.0003370000049471855;
//                postDiffusionSeedSliderValue = 0.0005009999731555581;
//                crossSeedSliderValue = 0.7950000166893005;
//                dryOutSliderValue = 0.9449997544288635;
//                predelayOutSliderValue = 0;
//                earlyOutSliderValue = 0.7250000238418579;
//                mainOutSliderValue = 0.6050001382827759;
//                hiPassEnabledButtonValue = 0;
//                lowPassEnabledButtonValue = 1;
//                lowShelfEnabledButtonValue = 0;
//                highShelfEnabledButtonValue = 1;
//                cutoffEnabledButtonValue = 0;
//                lateStageTapButtonValue = 1;
//                interpolationButtonValue = 1;
//
//
//            }
//
//            void AudioPluginAudioProcessor::initFactoryThroughTheLookingGlass()
//            {
//                //parameters from Through The Looking Glass in
//                //https://github.com/ValdemarOrn/CloudSeed/tree/master/Factory%20Programs
//                inputMixSliderValue = 0.0;
//                predelaySliderValue = 0.0;
//                lowCutSliderValue = 0.0;
//                highCutSliderValue = 0.74000012874603271;
//                tapCountSliderValue = 1.0;
//                tapLengthSliderValue = 1.0;
//                tapGainSliderValue = 1.0;
//                tapDecaySliderValue = 0.71000003814697266;
//                diffusionEnabledButtonValue = 1.0;
//                diffusionStagesSliderValue = 1.0;
//                diffusionDelaySliderValue = 0.65999996662139893;
//                diffusionFeedbackSliderValue = 0.76000010967254639;
//                lineCountSliderValue = 1.0;
//                lineDelaySliderValue = 0.9100002646446228;
//                lineDecaySliderValue = 0.80999958515167236;
//                lateDiffusionEnabledButtonValue = 1.0;
//                lateDiffusionStagesSliderValue = 1.0;
//                lateDiffusionDelaySliderValue = 0.71499955654144287;
//                lateDiffusionFeedbackSliderValue = 0.71999979019165039;
//                postLowShelfGainSliderValue = 0.87999987602233887;
//                postLowShelfFrequencySliderValue = 0.19499993324279785;
//                postHighShelfGainSliderValue = 0.72000008821487427;
//                postHighShelfFrequencySliderValue = 0.520000159740448;
//                postCutoffFrequencySliderValue = 0.7150002121925354;
//                earlyDiffusionModAmountSliderValue = 0.41999998688697815;
//                earlyDiffusionModRateSliderValue = 0.30500012636184692;
//                lineModAmountSliderValue = 0.4649999737739563;
//                lineModRateSliderValue = 0.3199998140335083;
//                lateDiffusionModAmountSliderValue = 0.40999993681907654;
//                lateDiffusionModRateSliderValue = 0.31500011682510376;
//                tapSeedSliderValue = 0.0003009999927598983;
//                diffusionSeedSliderValue = 0.00018899999849963933;
//                delaySeedSliderValue = 0.0001610000035725534;
//                postDiffusionSeedSliderValue = 0.00050099997315555811;
//                crossSeedSliderValue = 1.0;
//                dryOutSliderValue = 0.0;
//                predelayOutSliderValue = 0.0;
//                earlyOutSliderValue = 0.0;
//                mainOutSliderValue = 0.95499974489212036;
//                hiPassEnabledButtonValue = 0.0;
//                lowPassEnabledButtonValue = 1.0;
//                lowShelfEnabledButtonValue = 0.0;
//                highShelfEnabledButtonValue = 0.0;
//                cutoffEnabledButtonValue = 1.0;
//                lateStageTapButtonValue = 1.0;
//                interpolationButtonValue = 1.0;
//
//
//            }


//void AudioPluginAudioProcessor::loadPresetProgram(int index)
//{
//    switch(index){
//        case 0:
//            initChorusPreset();
//            break;
//        case 1:
//            initFactoryDullEchos();
//            break;
//        case 2:
//            initFactoryHyperplane();
//            break;
//        case 3:
//            initFactoryMediumSpace();
//            break;
//        case 4:
//            initFactoryNoiseInTheHallway();
//            break;
//        case 5:
//            initFactoryDullEchos();
//            break;
//        case 6:
//            initFactoryRubiKaFields();
//            break;
//        case 7:
//            initFactorySmallRoom();
//            break;
//        case 8:
//            initFactory90sAreBack();
//            break;
//        default:
//            break;
//    }
//
//    updateParameters();
//}




const juce::String AudioPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}


bool AudioPluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AudioPluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AudioPluginAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AudioPluginAudioProcessor::getTailLengthSeconds() const
{
    return 10.0;  // Reverb tail can be very long depending on decay settings
}

int AudioPluginAudioProcessor::getNumPrograms()
{
    return 9;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AudioPluginAudioProcessor::getCurrentProgram()
{
//    return currentProgramNumber;
    return 0;

}

void AudioPluginAudioProcessor::setCurrentProgram (int index)
{
//    if(index < 9){
//        currentProgramNumber = index;
//        loadPresetProgram(index);
//    }
    
    juce::ignoreUnused (index);

}


const juce::String AudioPluginAudioProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}


void AudioPluginAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}


//==============================================================================
void AudioPluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    reverb.SetSamplerate(sampleRate);
    
    //juce::ignoreUnused (sampleRate, samplesPerBlock);
}


void AudioPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}



bool AudioPluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // CloudSeed reverb only supports stereo processing
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // Input must match output (stereo in, stereo out)
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}



void AudioPluginAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                              juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused (midiMessages);

    juce::ScopedNoDenormals noDenormals;
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // Sync parameters from treeState (handles automation and preset changes)
    syncParametersFromTreeState();
    updateParameters();

    // we only process stereo signal, so we clear unused output buffers.
    for (auto i = 2; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // Copy buffer pointers to arrays with types matching ReverbController::Process
    const float* inputChannels[2] = { buffer.getReadPointer(0), buffer.getReadPointer(1) };
    float* outputChannels[2] = { buffer.getWritePointer(0), buffer.getWritePointer(1) };
    reverb.Process(inputChannels, outputChannels, buffer.getNumSamples());
}



void AudioPluginAudioProcessor::updateParameters()
{
    // todo: make individual call so only one parameter is updated
    reverb.SetParameter(Parameter::InputMix, inputMixSliderValue);
    reverb.SetParameter(Parameter::HighPass, lowCutSliderValue);
    reverb.SetParameter(Parameter::LowPass, highCutSliderValue);
    reverb.SetParameter(Parameter::PreDelay, predelaySliderValue);

    reverb.SetParameter(Parameter::DryOut, dryOutSliderValue);
    reverb.SetParameter(Parameter::PredelayOut, predelayOutSliderValue);
    reverb.SetParameter(Parameter::EarlyOut, earlyOutSliderValue);
    reverb.SetParameter(Parameter::MainOut, mainOutSliderValue);
    
    reverb.SetParameter(Parameter::TapCount, tapCountSliderValue);
    reverb.SetParameter(Parameter::TapLength, tapLengthSliderValue);
    reverb.SetParameter(Parameter::DiffusionDelay, diffusionDelaySliderValue);
    reverb.SetParameter(Parameter::DiffusionFeedback, diffusionFeedbackSliderValue);
    
    reverb.SetParameter(Parameter::TapGain, tapGainSliderValue);
    reverb.SetParameter(Parameter::TapDecay, tapDecaySliderValue);
    reverb.SetParameter(Parameter::EarlyDiffusionModAmount, earlyDiffusionModAmountSliderValue);
    reverb.SetParameter(Parameter::EarlyDiffusionModRate, earlyDiffusionModRateSliderValue);

    reverb.SetParameter(Parameter::DiffusionEnabled, diffusionEnabledButtonValue);
    reverb.SetParameter(Parameter::DiffusionStages, diffusionStagesSliderValue);
    reverb.SetParameter(Parameter::LineCount, lineCountSliderValue);
    
    reverb.SetParameter(Parameter::LineDelay, lineDelaySliderValue);
    reverb.SetParameter(Parameter::LineModAmount, lineModAmountSliderValue);
    reverb.SetParameter(Parameter::LineDecay, lineDecaySliderValue);
    reverb.SetParameter(Parameter::LineModRate, lineModRateSliderValue);
    
    reverb.SetParameter(Parameter::LateDiffusionEnabled, lateDiffusionEnabledButtonValue);
    reverb.SetParameter(Parameter::LateDiffusionStages, lateDiffusionStagesSliderValue);
    
    reverb.SetParameter(Parameter::LateDiffusionDelay, lateDiffusionDelaySliderValue);
    reverb.SetParameter(Parameter::LateDiffusionFeedback, lateDiffusionFeedbackSliderValue);
    reverb.SetParameter(Parameter::LateDiffusionModAmount, lateDiffusionModAmountSliderValue);
    reverb.SetParameter(Parameter::LateDiffusionModRate, lateDiffusionModRateSliderValue);
    
    reverb.SetParameter(Parameter::PostLowShelfGain, postLowShelfGainSliderValue);
    reverb.SetParameter(Parameter::PostLowShelfFrequency, postLowShelfFrequencySliderValue);
    reverb.SetParameter(Parameter::PostHighShelfGain, postHighShelfGainSliderValue);
    reverb.SetParameter(Parameter::PostHighShelfFrequency, postHighShelfFrequencySliderValue);
    reverb.SetParameter(Parameter::PostCutoffFrequency, postCutoffFrequencySliderValue);
    
    reverb.SetParameter(Parameter::TapSeed,tapSeedSliderValue);
    reverb.SetParameter(Parameter::DiffusionSeed, diffusionSeedSliderValue);
    reverb.SetParameter(Parameter::DelaySeed, delaySeedSliderValue);
    reverb.SetParameter(Parameter::PostDiffusionSeed, postDiffusionSeedSliderValue);
    reverb.SetParameter(Parameter::CrossSeed, crossSeedSliderValue);

    reverb.SetParameter(Parameter::HiPassEnabled, hiPassEnabledButtonValue);
    reverb.SetParameter(Parameter::LowPassEnabled, lowPassEnabledButtonValue);
    reverb.SetParameter(Parameter::LowShelfEnabled, lowShelfEnabledButtonValue);
    reverb.SetParameter(Parameter::HighShelfEnabled, highShelfEnabledButtonValue);
    reverb.SetParameter(Parameter::CutoffEnabled, cutoffEnabledButtonValue);
    
    reverb.SetParameter(Parameter::LateStageTap, lateStageTapButtonValue);
    reverb.SetParameter(Parameter::Interpolation, interpolationButtonValue);
}

void AudioPluginAudioProcessor::syncParametersFromTreeState()
{
    // Sync manual member variables from treeState
    // This ensures parameters loaded from presets/state are applied to the reverb engine

    inputMixSliderValue = *treeState.getRawParameterValue("inputMix");
    predelaySliderValue = *treeState.getRawParameterValue("predelay") / 1000.0f;
    lowCutSliderValue = *treeState.getRawParameterValue("lowCut");
    highCutSliderValue = *treeState.getRawParameterValue("highCut");

    dryOutSliderValue = *treeState.getRawParameterValue("dryOut");
    predelayOutSliderValue = *treeState.getRawParameterValue("predelayOut");
    earlyOutSliderValue = *treeState.getRawParameterValue("earlyOut");
    mainOutSliderValue = *treeState.getRawParameterValue("mainOut");

    tapCountSliderValue = *treeState.getRawParameterValue("tapCount");
    tapLengthSliderValue = *treeState.getRawParameterValue("tapLength") / 500.0f;
    diffusionDelaySliderValue = *treeState.getRawParameterValue("diffusionDelay");
    earlyDiffusionModAmountSliderValue = *treeState.getRawParameterValue("earlyDiffusionModAmount") * 0.4f;

    tapGainSliderValue = *treeState.getRawParameterValue("tapGain");
    tapDecaySliderValue = *treeState.getRawParameterValue("tapDecay");
    diffusionFeedbackSliderValue = *treeState.getRawParameterValue("diffusionFeedback");
    earlyDiffusionModRateSliderValue = *treeState.getRawParameterValue("earlyDiffusionModRate");

    lineDelaySliderValue = *treeState.getRawParameterValue("lineDelay");
    lineModAmountSliderValue = *treeState.getRawParameterValue("lineModAmount") * 0.4f;
    lateDiffusionDelaySliderValue = *treeState.getRawParameterValue("lateDiffusionDelay");
    lateDiffusionModAmountSliderValue = *treeState.getRawParameterValue("lateDiffusionModAmount") * 0.4f;

    lineDecaySliderValue = *treeState.getRawParameterValue("lineDecay");
    lineModRateSliderValue = *treeState.getRawParameterValue("lineModRate");
    lateDiffusionFeedbackSliderValue = *treeState.getRawParameterValue("lateDiffusionFeedback");
    lateDiffusionModRateSliderValue = *treeState.getRawParameterValue("lateDiffusionModRate");

    postLowShelfFrequencySliderValue = *treeState.getRawParameterValue("postLowShelfFrequency");
    postHighShelfFrequencySliderValue = *treeState.getRawParameterValue("postHighShelfFrequency");
    postCutoffFrequencySliderValue = *treeState.getRawParameterValue("postCutoffFrequency");

    postLowShelfGainSliderValue = *treeState.getRawParameterValue("postLowShelfGain");
    postHighShelfGainSliderValue = *treeState.getRawParameterValue("postHighShelfGain");
    crossSeedSliderValue = *treeState.getRawParameterValue("crossSeed");

    tapSeedSliderValue = *treeState.getRawParameterValue("tapSeed") / 1000000.0f + 0.001f;
    diffusionSeedSliderValue = *treeState.getRawParameterValue("diffusionSeed") / 1000000.0f + 0.001f;
    delaySeedSliderValue = *treeState.getRawParameterValue("delaySeed") / 1000000.0f + 0.001f;
    postDiffusionSeedSliderValue = *treeState.getRawParameterValue("postDiffusionSeed") / 1000000.0f + 0.001f;

    lineCountSliderValue = *treeState.getRawParameterValue("lineCount") / 12.0f;
    diffusionStagesSliderValue = *treeState.getRawParameterValue("diffusionStages") / 8.0f;
    lateDiffusionStagesSliderValue = *treeState.getRawParameterValue("lateDiffusionStages") / 8.0f;

    diffusionEnabledButtonValue = *treeState.getRawParameterValue("diffusionEnabled");
    lateDiffusionEnabledButtonValue = *treeState.getRawParameterValue("lateDiffusionEnabled");

    hiPassEnabledButtonValue = *treeState.getRawParameterValue("highPassEnabled");
    lowPassEnabledButtonValue = *treeState.getRawParameterValue("lowPassEnabled");
    cutoffEnabledButtonValue = *treeState.getRawParameterValue("cutoffEnabled");

    lowShelfEnabledButtonValue = *treeState.getRawParameterValue("lowShelfEnabled");
    highShelfEnabledButtonValue = *treeState.getRawParameterValue("highShelfEnabled");

    lateStageTapButtonValue = *treeState.getRawParameterValue("lateStageTap");
    interpolationButtonValue = *treeState.getRawParameterValue("interpolation");
}

//==============================================================================
bool AudioPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* AudioPluginAudioProcessor::createEditor()
{
    return new AudioPluginAudioProcessorEditor (*this);
}

//==============================================================================
void AudioPluginAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    
    auto state = treeState.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void AudioPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.

    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));

    if (xmlState != nullptr)
    {
        if (xmlState->hasTagName (treeState.state.getType()))
        {
            treeState.replaceState (juce::ValueTree::fromXml (*xmlState));
            // Sync member variables from loaded state and update reverb engine
            syncParametersFromTreeState();
            updateParameters();
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AudioPluginAudioProcessor();
}
