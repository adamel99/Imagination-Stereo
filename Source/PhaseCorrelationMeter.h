/*
  ==============================================================================

    PhaseCorrelationMeter.h
    Created: 16 Jun 2024 3:20:41am
    Author:  Adam Elhamami

  ==============================================================================
/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/
*/
class PhaseCorrelationMeter  : public juce::Component, private juce::Timer
{
public:
    PhaseCorrelationMeter();
    ~PhaseCorrelationMeter() override;

    void setAudioData(const std::vector<float>& leftChannelData, const std::vector<float>& rightChannelData);
    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    void timerCallback() override;
    float calculatePhaseCorrelation();

    juce::CriticalSection lock;
    std::vector<float> leftChannelData;
    std::vector<float> rightChannelData;
    float phaseCorrelation = 0.0f;
    std::vector<float> phaseHistory;
    
    bool isActive = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PhaseCorrelationMeter)
};
