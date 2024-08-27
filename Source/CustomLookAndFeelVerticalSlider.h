/*
  ==============================================================================

    CustomLookAndFeelVerticalSlider.h
    Created: 18 Jul 2024 9:16:31pm
    Author:  Adam Elhamami

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class CustomLookAndFeelVerticalSlider : public juce::LookAndFeel_V4
{
public:
    CustomLookAndFeelVerticalSlider();
    ~CustomLookAndFeelVerticalSlider() override;
    
    void drawLinearSlider (juce::Graphics&, int x, int y, int width, int height,
                           float sliderPos, float minSliderPos, float maxSliderPos,
                           const juce::Slider::SliderStyle, juce::Slider&) override;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CustomLookAndFeelVerticalSlider)
};

