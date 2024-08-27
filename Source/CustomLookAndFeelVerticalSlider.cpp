/*
  ==============================================================================

    CustomLookAndFeelVerticalSlider.cpp
    Created: 18 Jul 2024 9:16:31pm
    Author:  Adam Elhamami

  ==============================================================================
*/

#include "CustomLookAndFeelVerticalSlider.h"

CustomLookAndFeelVerticalSlider::CustomLookAndFeelVerticalSlider()
{
    // You can initialize any specific settings for this look and feel here if needed
}

CustomLookAndFeelVerticalSlider::~CustomLookAndFeelVerticalSlider()
{
    
}

void CustomLookAndFeelVerticalSlider::drawLinearSlider (juce::Graphics& g, int x, int y, int width, int height,
                                                        float sliderPos, float minSliderPos, float maxSliderPos,
                                                        const juce::Slider::SliderStyle style, juce::Slider& slider)
{
    const auto trackWidth = 6.0f;

    juce::Rectangle<float> track (x + width * 0.5f - trackWidth * 0.5f, y, trackWidth, height);
    g.setColour(juce::Colours::black);
    g.fillRoundedRectangle(track, trackWidth * 0.5f);

    juce::Rectangle<float> thumb (x + width * 0.5f - trackWidth * 1.5f, sliderPos - 10, trackWidth * 3.0f, 20);
    g.setColour(juce::Colours::black);
    g.fillRoundedRectangle(thumb, trackWidth * 0.5f);

    g.setColour(juce::Colour(236, 128, 165));
    g.drawRoundedRectangle(thumb, trackWidth * 0.5f, 2.0f);
}

