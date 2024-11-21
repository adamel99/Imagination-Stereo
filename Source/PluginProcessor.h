/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "VisualizerComponent.h"

//==============================================================================
/**
*/
class ImaginationAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    ImaginationAudioProcessor();
    ~ImaginationAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

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
    
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    juce::AudioProcessorValueTreeState apvts{ *this, nullptr, "Parameters", createParameterLayout() };
    
    // Public getter functions for the parameters
    juce::AudioParameterFloat* getWidthParam()           const { return width; }
    juce::AudioParameterFloat* getBalanceParam()         const { return balance; }
    juce::AudioParameterFloat* getInputGainParam()       const { return inputGain; }
    juce::AudioParameterFloat* getOutputGainParam()      const { return outputGain; }
    juce::AudioParameterFloat* getMidSideParam()         const { return midSide; }
    juce::AudioParameterFloat* getCrossfeedParam()       const { return crossfeed; }
    juce::AudioParameterFloat* getExciterEnhancerParam() const { return exciterEnhancer; }
    juce::AudioParameterFloat* getStereoSpreadParam()    const { return stereoSpread; }


private:
    juce::AudioParameterFloat* width           = nullptr;
    juce::AudioParameterFloat* balance         = nullptr;
    juce::AudioParameterFloat* inputGain       = nullptr;
    juce::AudioParameterFloat* outputGain      = nullptr;
    juce::AudioParameterFloat* midSide         = nullptr;
    juce::AudioParameterFloat* stereoSpread    = nullptr;
    juce::AudioParameterFloat* crossfeed       = nullptr;
    juce::AudioParameterFloat* exciterEnhancer = nullptr;

    void updateStereoImagerParams();
    
    juce::AudioBuffer<float> visualizerBuffer;
    VisualizerComponent visualizer;

    juce::dsp::DryWetMixer<float> dryWetMixer;
    juce::dsp::Gain<float> inputGainProcessor;
    juce::dsp::Gain<float> outputGainProcessor;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ImaginationAudioProcessor)
};
