/*
  ==============================================================================

    CustomLookAndFeel.cpp
    Created: 15 Jun 2024 12:35:49am
    Author:  Adam Elhamami

  ==============================================================================
*/

#include <JuceHeader.h>
#include "CustomLookAndFeel.h"

//==============================================================================
CustomLookAndFeel::CustomLookAndFeel()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

CustomLookAndFeel::~CustomLookAndFeel()
{
}

void CustomLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider)
{
    const float radius = juce::jmin(width / 2, height / 2) - 4.0f;
    const float centerX = x + width * 0.5f;
    const float centerY = y + height * 0.5f;
    const float rx = centerX - radius;
    const float ry = centerY - radius;
    const float rw = radius * 2.0f;
    const float angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);

    // Background Circle with subtle glowing effect
    juce::ColourGradient gradient(juce::Colours::black, centerX, centerY, juce::Colours::black.brighter(), centerX, centerY - radius, true);
    g.setGradientFill(gradient);
    g.fillEllipse(rx, ry, rw, rw);

    // Outer glowing arc
    juce::Path arc;
    arc.addCentredArc(centerX, centerY, radius, radius, 0.0f, rotaryStartAngle, angle, true);
    juce::ColourGradient arcGradient(juce::Colour(254, 93, 159), centerX, centerY, juce::Colour(254, 93, 159), centerX, centerY - radius, true);
    arcGradient.addColour(0.5, juce::Colour(254, 93, 159));
    g.setGradientFill(arcGradient);
    g.strokePath(arc, juce::PathStrokeType(4.0f));

    // Glow effect on the arc
    g.setColour(juce::Colour(254, 93, 159).withAlpha(0.2f));
    g.fillPath(arc);

    // Manually drawing the knob shadow for a 3D effect
    g.setColour(juce::Colours::black.withAlpha(0.6f));
    for (int i = 0; i < 10; ++i)
    {
        g.drawEllipse(centerX - radius * 0.7f + i * 0.5f, centerY - radius * 0.7f + i * 0.5f, radius * 1.4f, radius * 1.4f, 1.0f);
    }

    // Knob with realistic shading
    juce::Colour knobColor = juce::Colours::darkgrey;
    juce::ColourGradient knobGradient(knobColor.brighter(), centerX, centerY - radius * 0.4f, knobColor.darker(0.4f), centerX, centerY + radius * 0.4f, true);
    g.setGradientFill(knobGradient);
    g.fillEllipse(centerX - radius * 0.7f, centerY - radius * 0.7f, radius * 1.4f, radius * 1.4f);

    // Inner glow effect
    juce::Colour innerGlowColor = juce::Colour(254, 93, 159);
    g.setColour(innerGlowColor.withAlpha(0.3f));
    g.drawEllipse(centerX - radius * 0.5f, centerY - radius * 0.5f, radius, radius, 3.0f);

    // Knob highlight for 3D effect
    juce::Colour highlightColor = juce::Colour(254, 93, 159);
    g.setColour(highlightColor.withAlpha(0.5f));
    g.fillEllipse(centerX - radius * 0.5f, centerY - radius * 0.5f, radius * 1.0f, radius * 1.0f);

    // Knob indicator with reflection effect
    juce::Path indicator;
    const float indicatorLength = radius * 0.8f;
    const float indicatorWidth = 4.0f;
    indicator.addRectangle(-indicatorWidth / 2, -indicatorLength, indicatorWidth, indicatorLength);
    g.setColour(juce::Colour(246, 134, 189));
    g.fillPath(indicator, juce::AffineTransform::rotation(angle).translated(centerX, centerY));

    // Adding reflection to the knob for a more realistic effect
    juce::ColourGradient reflectionGradient(juce::Colours::white.withAlpha(0.5f), centerX, centerY - radius * 0.5f, juce::Colours::transparentWhite, centerX, centerY + radius * 0.5f, true);
    g.setGradientFill(reflectionGradient);
    g.fillEllipse(centerX - radius * 0.7f, centerY - radius * 0.7f, radius * 1.4f, radius * 1.4f);
}


void CustomLookAndFeel::drawLabel(juce::Graphics& g, juce::Label& label)
{
    // Fill background with a transparent color
    g.fillAll(label.findColour(juce::Label::backgroundColourId).withAlpha(0.0f));
    
    // Set text color
    g.setColour(label.findColour(juce::Label::textColourId));
    
    // Draw the text
    g.drawText(label.getText(), label.getLocalBounds(), label.getJustificationType(), true);
    
    // Draw border if needed (omitted for a borderless text box)
}
