/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "VisualizerComponent.h"
#include <optional>
#include "CustomLookAndFeel.h"
#include "PhaseCorrelationMeter.h"
#include "CustomLookAndFeelVerticalSlider.h"
//==============================================================================
/**
*/
class ImaginationAudioProcessorEditor  : public juce::AudioProcessorEditor
                                        
{
public:
    ImaginationAudioProcessorEditor (ImaginationAudioProcessor&);
    ~ImaginationAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    VisualizerComponent visualizer;
    PhaseCorrelationMeter phaseCorrelationMeter;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    ImaginationAudioProcessor& audioProcessor;

    juce::Slider widthSlider;
    juce::Slider balanceSlider;
    juce::Slider inputGainSlider;
    juce::Slider outputGainSlider;
    juce::Slider midSideSlider;
//    juce::Slider depthSlider;
    juce::Slider crossfeedSlider;
    juce::Slider exciterEnhancerSlider;

    juce::Label widthLabel;
    juce::Label balanceLabel;
    juce::Label inputGainLabel;
    juce::Label outputGainLabel;
    juce::Label midSideLabel;
//    juce::Label depthLabel;
    juce::Label crossfeedLabel;
    juce::Label exciterEnhancerLabel;
    
    std::optional<juce::AudioProcessorValueTreeState::SliderAttachment> widthAttachment;
    std::optional<juce::AudioProcessorValueTreeState::SliderAttachment> balanceAttachment;
    std::optional<juce::AudioProcessorValueTreeState::SliderAttachment> inputGainAttachment;
    std::optional<juce::AudioProcessorValueTreeState::SliderAttachment> outputGainAttachment;
    std::optional<juce::AudioProcessorValueTreeState::SliderAttachment> midSideAttachment;
//    std::optional<juce::AudioProcessorValueTreeState::SliderAttachment> depthAttachment;
    std::optional<juce::AudioProcessorValueTreeState::SliderAttachment> crossfeedAttachment;
    std::optional<juce::AudioProcessorValueTreeState::SliderAttachment> exciterEnhancerAttachment;

    CustomLookAndFeel customLookAndFeel;
    CustomLookAndFeelVerticalSlider customLookAndFeelVerticalSlider;
    
    juce::Image backgroundImage;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ImaginationAudioProcessorEditor)
};
