/*
  ==============================================================================

    VisualizerComponent.cpp
    Created: 4 Jun 2024 7:30:34pm
    Author:  Adam Elhamami

  ==============================================================================
*/

#include <JuceHeader.h>
#include "VisualizerComponent.h"

//==============================================================================
VisualizerComponent::VisualizerComponent()
: maxHistorySize(10)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    startTimer(30);
    
}

VisualizerComponent::~VisualizerComponent()
{
    stopTimer();
}

void VisualizerComponent::setAudioData(const std::vector<float>& leftChannelData, const std::vector<float>& rightChannelData)
{
    const juce::ScopedLock sl(lock);
    this->leftChannelData = leftChannelData;
    this->rightChannelData = rightChannelData;
}

void VisualizerComponent::paint (juce::Graphics& g)
{
    g.setImageResamplingQuality(juce::Graphics::ResamplingQuality::highResamplingQuality); // Ensure high-quality resampling
    g.fillAll(juce::Colour(20, 20, 25).withAlpha(0.5f));

    drawGrid(g);
    drawStereoImage(g);
    drawLabels(g);
}

void VisualizerComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
}

void VisualizerComponent::copyAudioBuffer(const juce::AudioBuffer<float>& buffer)
{
    const juce::ScopedLock sl(lock);

    int numSamples = buffer.getNumSamples();
    int numChannels = buffer.getNumChannels();

    leftChannelData.resize(numSamples);
    rightChannelData.resize(numSamples);

    if (numChannels > 0)
        std::copy(buffer.getReadPointer(0), buffer.getReadPointer(0) + numSamples, leftChannelData.begin());

    if (numChannels > 1)
        std::copy(buffer.getReadPointer(1), buffer.getReadPointer(1) + numSamples, rightChannelData.begin());
    else
        rightChannelData.assign(numSamples, 0.0f); // Handle mono input by setting rightChannelData to 0
}

void VisualizerComponent::timerCallback()
{
    repaint();
}

void VisualizerComponent::drawGrid(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    auto center = bounds.getCentre();
    float radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) / 2 + 90.0f;

    // Fill with fully transparent color
    g.fillAll(juce::Colours::transparentBlack.withAlpha(0.0f)); // Fully transparent

    // Draw sphere gradient
    juce::ColourGradient sphereGradient(juce::Colour::fromRGB(10, 10, 100), center,
                                        juce::Colours::transparentBlack.withAlpha(0.0f), center.withX(center.x + radius / 2).withY(center.y + radius / 2), true);
    sphereGradient.addColour(0.3, juce::Colour::fromRGB(30, 30, 150));
    sphereGradient.addColour(0.5, juce::Colour::fromRGB(70, 70, 200));
    sphereGradient.addColour(0.7, juce::Colour::fromRGB(150, 150, 255));
    sphereGradient.addColour(1.0, juce::Colour::fromRGB(200, 200, 255));
    g.setGradientFill(sphereGradient);
    g.fillEllipse(center.x - radius, center.y - radius, 2 * radius, 2 * radius);

    // Add highlights for the glossy effect
    auto drawHighlight = [&](float offsetX, float offsetY, float width, float height, juce::Colour colour)
    {
        juce::ColourGradient highlightGradient(colour.withAlpha(0.8f), center.withX(center.x + offsetX).withY(center.y + offsetY),
                                               juce::Colours::transparentWhite, center.withX(center.x + offsetX * 2.0f).withY(center.y + offsetY * 2.0f), true);
        g.setGradientFill(highlightGradient);
        g.fillEllipse(center.x + offsetX - width / 2.0f, center.y + offsetY - height / 2.0f, width, height);
    };

    // Add multiple highlights for the 3D effect
//    drawHighlight(-radius / 4.0f, -radius / 4.0f, radius * 0.8f, radius * 0.8f, juce::Colours::white);
    drawHighlight(radius / 5.0f, -radius / 5.0f, radius * 0.6f, radius * 0.6f, juce::Colours::lightblue.withAlpha(0.5f));
//    drawHighlight(-radius / 6.0f, radius / 6.0f, radius * 0.4f, radius * 0.4f, juce::Colours::skyblue.withAlpha(0.3f));

    // Draw subtle glow effect around the sphere
    juce::ColourGradient outerGlowGradient(juce::Colours::transparentWhite, center,
                                           juce::Colours::transparentBlack.withAlpha(0.0f).withAlpha(0.1f), center.withY(center.y + radius * 1.5f), true);
    g.setGradientFill(outerGlowGradient);
    g.fillEllipse(center.x - radius * 1.5f, center.y - radius * 1.5f, 3 * radius, 3 * radius);
}

void VisualizerComponent::drawStereoImage(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
       auto center = bounds.getCentre();
       float maxRadius = juce::jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f - 40.0f;

    juce::Path currentPath;
    const juce::ScopedLock sl(lock);

       const int smoothingWindowSize = 20;  // Increase for smoother lines
       std::vector<float> smoothedLeft(leftChannelData.size());
       std::vector<float> smoothedRight(rightChannelData.size());

       for (size_t i = 0; i < leftChannelData.size(); ++i)
       {
           float leftSum = 0.0f;
           float rightSum = 0.0f;
           int count = 0;

           for (int j = -smoothingWindowSize; j <= smoothingWindowSize; ++j)
           {
               int index = static_cast<int>(i) + j;
               if (index >= 0 && index < static_cast<int>(leftChannelData.size()))
               {
                   leftSum += leftChannelData[index];
                   rightSum += rightChannelData[index];
                   ++count;
               }
           }

           smoothedLeft[i] = leftSum / count;
           smoothedRight[i] = rightSum / count;
       }

    for (size_t i = 0; i < smoothedLeft.size() && i < smoothedRight.size(); i += 2) // Skip every other sample for optimization
    {
        float left = smoothedLeft[i];
        float right = smoothedRight[i];

        // Calculate stereo width and amplitude
        float stereoWidth = left - right;
        float amplitude = (left + right) * 0.5f;

        // Map stereo width to horizontal displacement and amplitude to vertical displacement
        float x = juce::jmap(stereoWidth, -1.0f, 1.0f, -maxRadius, maxRadius);
        float y = juce::jmap(amplitude, -1.0f, 1.0f, maxRadius, -maxRadius);

        // Clamp the coordinates to stay within the sphere's radius
        float distance = std::sqrt(x * x + y * y);
        if (distance > maxRadius)
        {
            float scale = maxRadius / distance;
            x *= scale;
            y *= scale;
        }

        // Starting from the center
        x += center.x;
        y += center.y;

        if (i == 0)
            currentPath.startNewSubPath(x, y);
        else
            currentPath.lineTo(x, y);
    }

    pathHistory.push_back(currentPath);
    if (pathHistory.size() > maxHistorySize)
    pathHistory.pop_front(); // Limit history to avoid excessive memory usage

    // Enable anti-aliasing
    g.setImageResamplingQuality(juce::Graphics::ResamplingQuality::highResamplingQuality);

    // Draw the main path with gradient color from pink to deep red
       juce::ColourGradient pathGradient(juce::Colours::pink.withAlpha(0.8f), center,
                                         juce::Colours::red.withAlpha(0.8f), center.withX(center.x + maxRadius), true);
       g.setGradientFill(pathGradient);
       g.strokePath(currentPath, juce::PathStrokeType(2.0f));

       // Optimize glow effect by reducing the number of layers or simplifying the gradient
       int glowLayers = 3;  // Reduced layers for performance
       float alphaStep = 0.1f / static_cast<float>(glowLayers);

       for (int j = 0; j < glowLayers; ++j)
       {
           float alphaMultiplier = 0.1f * (glowLayers - j);
           for (size_t i = 0; i < pathHistory.size(); ++i)
           {
               juce::ColourGradient glowGradient(juce::Colours::pink.withAlpha(alphaMultiplier * static_cast<float>(i + 1) * alphaStep), center,
                                                 juce::Colours::red.withAlpha(alphaMultiplier * static_cast<float>(i + 1) * alphaStep), center.withX(center.x + maxRadius), true);
               g.setGradientFill(glowGradient);
               g.strokePath(pathHistory[i], juce::PathStrokeType(3.0f + j * 0.5f));
           }
       }
       
       // Simplify highlights and shading for better performance
       g.setGradientFill(juce::ColourGradient(juce::Colours::white.withAlpha(0.3f), center.withX(center.x + maxRadius * 0.5f),
                                              juce::Colours::transparentWhite, center.withY(center.y + maxRadius * 0.5f), true));
       g.strokePath(currentPath, juce::PathStrokeType(1.0f));

    // Add subtle highlights on the path for a more polished look
    g.setGradientFill(juce::ColourGradient(juce::Colours::white.withAlpha(0.4f), center.withX(center.x + maxRadius * 0.5f),
                                           juce::Colours::transparentWhite, center.withY(center.y + maxRadius * 0.5f), true));
    g.strokePath(currentPath, juce::PathStrokeType(1.0f));

    // Add shading to create depth effect
    g.setGradientFill(juce::ColourGradient(juce::Colours::black.withAlpha(0.5f), center,
                                           juce::Colours::transparentBlack, center.withY(center.y + maxRadius), true));
    g.fillPath(currentPath);
}

void VisualizerComponent::drawLabels(juce::Graphics& g)
{
    //Set Font
    juce::Typeface::Ptr helvetica = juce::Typeface::createSystemTypefaceFor(BinaryData::HelveticaBold_woff, BinaryData::HelveticaBold_woffSize);
    juce::Font font(*helvetica);
    
    auto bounds = getLocalBounds().toFloat();
    auto center = bounds.getCentre();
    float radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f - 10.0f;

    g.setColour(juce::Colour(247, 236, 242));
//    g.drawText("MONO", center.x - 30, center.y - radius - 20, 60, 20, juce::Justification::centred);
    g.drawText("L", center.x - radius - 20, center.y - 10, 20, 20, juce::Justification::centred);
    g.drawText("R", center.x + radius, center.y - 10, 20, 20, juce::Justification::centred);
    
    g.setFont(font);
}
