/*
  ==============================================================================

    PhaseCorrelationMeter.cpp
    Created: 16 Jun 2024 3:20:41am
    Author:  Adam Elhamami

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PhaseCorrelationMeter.h"

//==============================================================================
PhaseCorrelationMeter::PhaseCorrelationMeter()
{
    startTimerHz(30); // Update every 100ms instead of 40ms
}

PhaseCorrelationMeter::~PhaseCorrelationMeter()
{
    stopTimer();
}

void PhaseCorrelationMeter::setAudioData(const std::vector<float>& leftChannelData, const std::vector<float>& rightChannelData)
{
    const juce::ScopedLock sl(lock);
        this->leftChannelData = leftChannelData;
        this->rightChannelData = rightChannelData;

        if (!leftChannelData.empty() && !rightChannelData.empty())
        {
            phaseCorrelation = calculatePhaseCorrelation();
            isActive = true;
        }
        else
        {
            isActive = false;
        }
}

void PhaseCorrelationMeter::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();

        g.fillAll(juce::Colour(60, 60, 65));

        // Draw the background of the meter
        g.setColour(juce::Colours::black);
        g.fillRect(bounds);

        if (isActive)
        {
            // Draw grid lines
            g.setColour(juce::Colour(40, 40, 45));
            for (int i = 1; i < 10; ++i)
            {
                float x = bounds.getWidth() / 10 * i;
                g.drawLine(x, 0, x, bounds.getHeight());
            }
            for (int i = 1; i < 5; ++i)
            {
                float y = bounds.getHeight() / 5 * i;
                g.drawLine(0, y, bounds.getWidth(), y);
            }

            // Draw the waveform
            juce::Path waveformPath;
            float yCenter = bounds.getCentreY();
            waveformPath.startNewSubPath(0, yCenter);

            {
                const juce::ScopedLock sl(lock);
                for (size_t i = 0; i < phaseHistory.size(); ++i)
                {
                    float x = juce::jmap(static_cast<float>(i), 0.0f, static_cast<float>(phaseHistory.size() - 1), 0.0f, bounds.getWidth());
                    float y = juce::jmap(phaseHistory[i], -1.0f, 1.0f, bounds.getBottom(), bounds.getY());
                    waveformPath.lineTo(x, y);
                }
            }

            juce::ColourGradient gradient(juce::Colour(246, 134, 189), bounds.getX(), bounds.getY(),
                    juce::Colour::fromRGB(70, 70, 200), bounds.getRight(), bounds.getY(), false);
            gradient.addColour(0.5, juce::Colours::white);
            g.setGradientFill(gradient);
            g.strokePath(waveformPath, juce::PathStrokeType(2.0f));

            // Draw the text labels
            g.setColour(juce::Colours::white);
            g.setFont(15.0f);
            g.drawText("-1.00", bounds.removeFromLeft(40), juce::Justification::centred);
            g.drawText("1.00", bounds.removeFromRight(40), juce::Justification::centred);
        }
        else
        {
            // Draw inactive state (e.g., no audio playing)
            g.setColour(juce::Colours::darkgrey);
            g.setFont(15.0f);
            g.drawText("No Audio", bounds, juce::Justification::centred);
        }
}

void PhaseCorrelationMeter::resized()
{
}

void PhaseCorrelationMeter::timerCallback()
{
    const juce::ScopedLock sl(lock);
      
      if (isActive)
      {
          phaseCorrelation = calculatePhaseCorrelation();
          phaseHistory.push_back(phaseCorrelation);

          if (phaseHistory.size() > 100) // Keep a fixed size history
          {
              phaseHistory.erase(phaseHistory.begin());
          }

          repaint();
      }
}

float PhaseCorrelationMeter::calculatePhaseCorrelation()
{
    if (leftChannelData.size() != rightChannelData.size() || leftChannelData.empty())
           return 0.0f;

       // Downsample or use only a portion of the data
       size_t step = std::max<size_t>(leftChannelData.size() / 1000, 1);
       float sumProduct = 0.0f;
       float sumLeft = 0.0f;
       float sumRight = 0.0f;

       for (size_t i = 0; i < leftChannelData.size(); i += step)
       {
           sumProduct += leftChannelData[i] * rightChannelData[i];
           sumLeft += leftChannelData[i] * leftChannelData[i];
           sumRight += rightChannelData[i] * rightChannelData[i];
       }

       if (sumLeft == 0.0f || sumRight == 0.0f)
           return 0.0f;

       return sumProduct / (std::sqrt(sumLeft) * std::sqrt(sumRight));
}
