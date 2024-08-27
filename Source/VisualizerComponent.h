/*
  ==============================================================================

    VisualizerComponent.h
    Created: 4 Jun 2024 7:30:34pm
    Author:  Adam Elhamami

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>
#include <deque>
#include <OpenGl/gl.h>



//==============================================================================
/*
*/
class VisualizerComponent  : public juce::Component, private juce::Timer
{
public:
    VisualizerComponent();
    ~VisualizerComponent() override;
    
    void setAudioData(const std::vector<float>& leftChannelData, const std::vector<float>& rightChannelData);
    void copyAudioBuffer(const juce::AudioBuffer<float>& buffer);

    void paint(juce::Graphics& g) override;
    void resized() override;
    
private:
    
    void timerCallback() override;
    void drawGrid(juce::Graphics& g);
    void drawStereoImage(juce::Graphics& g);
    void drawLabels(juce::Graphics& g);


    juce::CriticalSection lock;
    std::vector<float> leftChannelData;
    std::vector<float> rightChannelData;

    std::deque<juce::Path> pathHistory; // New data member to store paths
    int maxHistorySize = 20; // Maximum number of paths to store

    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VisualizerComponent)
};
