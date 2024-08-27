/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Param.h"
#include "CustomLookAndFeel.h"
#include "BinaryData.h"
//#include "helvetica-255-webfront/Helvetica.woff"
#include "CustomLookAndFeelVerticalSlider.h"

//==============================================================================
ImaginationAudioProcessorEditor::ImaginationAudioProcessorEditor (ImaginationAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize(800, 500);
    
    widthSlider.setLookAndFeel(&customLookAndFeelVerticalSlider);
    balanceSlider.setLookAndFeel(&customLookAndFeel);
    inputGainSlider.setLookAndFeel(&customLookAndFeel);
    outputGainSlider.setLookAndFeel(&customLookAndFeel);
    midSideSlider.setLookAndFeel(&customLookAndFeel);

    addAndMakeVisible(widthSlider);
    widthSlider.setSliderStyle(juce::Slider::LinearVertical);
    widthSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    widthSlider.setRange(0.0, 100.0, 0.01);
    widthSlider.setValue(*audioProcessor.getWidthParam());

    addAndMakeVisible(balanceSlider);
    balanceSlider.setSliderStyle(juce::Slider::Rotary);
    balanceSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    balanceSlider.setRange(-1.0, 1.0, 0.01);
    balanceSlider.setValue(*audioProcessor.getBalanceParam());

    addAndMakeVisible(inputGainSlider);
    inputGainSlider.setSliderStyle(juce::Slider::Rotary);
    inputGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    inputGainSlider.setRange(-60.0, 24.0, 0.01);
    inputGainSlider.setValue(*audioProcessor.getInputGainParam());
    
    addAndMakeVisible(outputGainSlider);
    outputGainSlider.setSliderStyle(juce::Slider::Rotary);
    outputGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    outputGainSlider.setRange(-60.0, 24.0, 0.01);
    outputGainSlider.setValue(*audioProcessor.getOutputGainParam());
    
    addAndMakeVisible(midSideSlider);
    midSideSlider.setSliderStyle(juce::Slider::Rotary);
    midSideSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    midSideSlider.setRange(-60.0, 24.0, 0.01);
    midSideSlider.setValue(*audioProcessor.getOutputGainParam());
    
//    addAndMakeVisible(depthSlider);
//    depthSlider.setSliderStyle(juce::Slider::Rotary);
//    depthSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
//    depthSlider.setRange(-60.0, 24.0, 0.01);
//    depthSlider.setValue(*audioProcessor.getOutputGainParam());

    addAndMakeVisible(widthLabel);
    widthLabel.setText("WIDTH", juce::dontSendNotification);
    widthLabel.attachToComponent(&widthSlider, false);

    addAndMakeVisible(balanceLabel);
    balanceLabel.setText("BALANCE", juce::dontSendNotification);
    balanceLabel.attachToComponent(&balanceSlider, false);

    addAndMakeVisible(inputGainLabel);
    inputGainLabel.setText("INPUT", juce::dontSendNotification);
    inputGainLabel.attachToComponent(&inputGainSlider, false);

    addAndMakeVisible(outputGainLabel);
    outputGainLabel.setText("OUTPUT", juce::dontSendNotification);
    outputGainLabel.attachToComponent(&outputGainSlider, false);

    addAndMakeVisible(midSideLabel);
    midSideLabel.setText("MID/SIDE", juce::dontSendNotification);
    midSideLabel.attachToComponent(&midSideSlider, false);
    
    addAndMakeVisible(crossfeedLabel);
    crossfeedLabel.setText("CROSSFEED", juce::dontSendNotification);
    crossfeedLabel.attachToComponent(&crossfeedSlider, false);
    
    addAndMakeVisible(exciterEnhancerLabel);
    exciterEnhancerLabel.setText("EXCITER", juce::dontSendNotification);
    exciterEnhancerLabel.attachToComponent(&exciterEnhancerSlider, false);

    addAndMakeVisible(visualizer);
    addAndMakeVisible(phaseCorrelationMeter);

    widthAttachment.emplace(audioProcessor.apvts, ParamIDs::width, widthSlider);
    balanceAttachment.emplace(audioProcessor.apvts, ParamIDs::balance, balanceSlider);
    inputGainAttachment.emplace(audioProcessor.apvts, ParamIDs::inputGain, inputGainSlider);
    outputGainAttachment.emplace(audioProcessor.apvts, ParamIDs::outputGain, outputGainSlider);
    midSideAttachment.emplace(audioProcessor.apvts, ParamIDs::midSide, midSideSlider);
//    depthAttachment.emplace(audioProcessor.apvts, ParamIDs::depth, depthSlider);
    
    crossfeedSlider.setLookAndFeel(&customLookAndFeel);
    exciterEnhancerSlider.setLookAndFeel(&customLookAndFeel);

    addAndMakeVisible(crossfeedSlider);
    crossfeedSlider.setSliderStyle(juce::Slider::Rotary);
    crossfeedSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    crossfeedSlider.setRange(0.0, 1.0, 0.01);
    crossfeedSlider.setValue(*audioProcessor.getCrossfeedParam());

    addAndMakeVisible(exciterEnhancerSlider);
    exciterEnhancerSlider.setSliderStyle(juce::Slider::Rotary);
    exciterEnhancerSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    exciterEnhancerSlider.setRange(0.0, 1.0, 0.01);
    exciterEnhancerSlider.setValue(*audioProcessor.getExciterEnhancerParam());

    crossfeedAttachment.emplace(audioProcessor.apvts, "crossfeed", crossfeedSlider);
    exciterEnhancerAttachment.emplace(audioProcessor.apvts, "exciterEnhancer", exciterEnhancerSlider);
    
    // Set Font
    juce::Typeface::Ptr helvetica = juce::Typeface::createSystemTypefaceFor(BinaryData::HelveticaBold_woff, BinaryData::HelveticaBold_woffSize);
    juce::Font font(*helvetica);
    
    widthLabel.setFont(font);
    balanceLabel.setFont(font);
    inputGainLabel.setFont(font);
    outputGainLabel.setFont(font);
    midSideLabel.setFont(font);
//    depthLabel.setFont(font);
    crossfeedLabel.setFont(font);
    exciterEnhancerLabel.setFont(font);
       
}

ImaginationAudioProcessorEditor::~ImaginationAudioProcessorEditor()
{
    widthSlider.setLookAndFeel(nullptr);
    balanceSlider.setLookAndFeel(nullptr);
    inputGainSlider.setLookAndFeel(nullptr);
    outputGainSlider.setLookAndFeel(nullptr);
    midSideSlider.setLookAndFeel(nullptr);
//    depthSlider.setLookAndFeel(nullptr);
    crossfeedSlider.setLookAndFeel(nullptr);
    exciterEnhancerSlider.setLookAndFeel(nullptr);
}

//==============================================================================
void ImaginationAudioProcessorEditor::paint (juce::Graphics& g)
{
//     Load the image from memory
//    juce::Image backgroundImage = juce::ImageCache::getFromMemory(BinaryData::fe36a0a008a9654494bd357bb1b564a3_jpg, BinaryData::fe36a0a008a9654494bd357bb1b564a3_jpgSize);
////
////    // Check if the image is valid
//    if (!backgroundImage.isNull())
//    {
////        // Draw the image to fill the entire component area
//        g.drawImage(backgroundImage, getLocalBounds().toFloat());
//    }
//    else
//    {
//         Fallback to a solid colour if the image failed to load
        g.fillAll(juce::Colours::black);
//    }
    // Draw "platform" backgrounds for components with gradients
    auto area = getLocalBounds().reduced(20);
       auto sliderWidth = 80;
       auto sliderHeight = 75;
       auto visualizerHeight = area.getHeight() - sliderHeight - 40;
       auto visualizerWidth = area.getWidth() - (2 * sliderWidth + 200);
       auto phaseMeterHeight = 100;

       auto leftArea = area.removeFromLeft(sliderWidth * 2 + 20);
       auto rightArea = area.removeFromRight(sliderWidth);
       auto visualizerArea = area.removeFromTop(visualizerHeight).withSizeKeepingCentre(visualizerWidth, visualizerHeight);
       auto phaseMeterArea = area.removeFromBottom(phaseMeterHeight).withSizeKeepingCentre(visualizerWidth, phaseMeterHeight);

       auto drawLiftedPlatform = [&](juce::Rectangle<int> area) {
           juce::Rectangle<float> platformArea = area.toFloat();

           // Background shadow
           g.setColour(juce::Colours::black.withAlpha(0.3f));
           g.fillRoundedRectangle(platformArea.translated(3, 5), 10.0f);

           // Platform background with gradient
           juce::ColourGradient gradient(juce::Colours::black.brighter(0.1f), 0, 0,
                                         juce::Colours::black.darker(0.3f), 0, platformArea.getHeight(), false);
           gradient.addColour(0.5, juce::Colours::darkgrey.darker(0.2f));
           g.setGradientFill(gradient);
           g.fillRoundedRectangle(platformArea, 10.0f);

           // Highlight
           g.setColour(juce::Colours::white.withAlpha(0.1f));
           g.drawRoundedRectangle(platformArea, 10.0f, 1.5f);

           // Inner shadow for further depth
           juce::DropShadow innerShadow(juce::Colours::black.withAlpha(0.5f), 5, {});
           innerShadow.drawForRectangle(g, platformArea.reduced(3).toNearestInt());

           // Top edge highlight
           juce::Path edgeHighlight;
           edgeHighlight.addRoundedRectangle(platformArea.reduced(1.0f, 1.0f), 10.0f);
           g.setGradientFill(juce::ColourGradient(juce::Colours::white.withAlpha(0.1f), platformArea.getX(), platformArea.getY(),
                                                  juce::Colours::transparentWhite, platformArea.getX(), platformArea.getBottom(), false));
           g.strokePath(edgeHighlight, juce::PathStrokeType(1.0f));
       };

       drawLiftedPlatform(visualizerArea);
       drawLiftedPlatform(phaseMeterArea);
       drawLiftedPlatform(leftArea);
       drawLiftedPlatform(rightArea);
   }

void ImaginationAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(20);
    auto sliderWidth = 80;
    auto sliderHeight = 75;
    auto labelHeight = 20;
    auto visualizerHeight = area.getHeight() - sliderHeight - 40;
    auto visualizerWidth = area.getWidth() - (2 * sliderWidth + 200);
    auto phaseMeterHeight = 100;
    
    auto leftArea = area.removeFromLeft(sliderWidth * 2 + 20); // Adjusted for two columns of sliders
    auto rightArea = area.removeFromRight(sliderWidth);
    auto visualizerArea = area.removeFromTop(visualizerHeight).withSizeKeepingCentre(visualizerWidth, visualizerHeight);
    auto phaseMeterArea = area.removeFromBottom(phaseMeterHeight).withSizeKeepingCentre(visualizerWidth, phaseMeterHeight);
    
    visualizer.setBounds(visualizerArea);
    phaseCorrelationMeter.setBounds(phaseMeterArea);
    
    // Calculate positions for left sliders (two columns)
    int numLeftSliders = 6;
    int numColumns = 2;
    int numRows = (numLeftSliders + 1) / numColumns; // Calculate number of rows needed
    
    int verticalSpacing = (leftArea.getHeight() - numRows * sliderHeight) / (numRows + 1);
    int horizontalSpacing = (leftArea.getWidth() - numColumns * sliderWidth) / (numColumns + 1);
    
    auto leftColumn = leftArea.removeFromLeft(leftArea.getWidth() / 2);
    auto rightColumn = leftArea;
    
    // Position sliders and labels in two columns
    std::vector<juce::Component*> leftColumnComponents = {
        &balanceSlider, &balanceLabel,
        &inputGainSlider, &inputGainLabel,
        &outputGainSlider, &outputGainLabel
    };
    std::vector<juce::Component*> rightColumnComponents = {
        &midSideSlider, &midSideLabel,
        &crossfeedSlider, &crossfeedLabel,
        &exciterEnhancerSlider, &exciterEnhancerLabel
    };
    
    int currentY = verticalSpacing;
    for (int i = 0; i < leftColumnComponents.size(); i += 2)
    {
        leftColumnComponents[i]->setBounds(leftColumn.getX() + horizontalSpacing, currentY, sliderWidth, sliderHeight);
        leftColumnComponents[i + 1]->setBounds(leftColumn.getX() + horizontalSpacing, currentY + sliderHeight + 5, sliderWidth, labelHeight);
        currentY += sliderHeight + labelHeight + verticalSpacing;
    }
    
    currentY = verticalSpacing;
    for (int i = 0; i < rightColumnComponents.size(); i += 2)
    {
        rightColumnComponents[i]->setBounds(rightColumn.getX() + horizontalSpacing, currentY, sliderWidth, sliderHeight);
        rightColumnComponents[i + 1]->setBounds(rightColumn.getX() + horizontalSpacing, currentY + sliderHeight + 5, sliderWidth, labelHeight);
        currentY += sliderHeight + labelHeight + verticalSpacing;
    }
    
    // Position the width slider and label on the right
    int rightLabelTopPadding = 30;
    int rightLabelHeight = 15;
    int rightSliderTopPadding = rightLabelTopPadding + rightLabelHeight + 5;
    
    widthLabel.setBounds(rightArea.getX(), rightLabelTopPadding, sliderWidth, rightLabelHeight);
    widthSlider.setBounds(rightArea.getX(), rightSliderTopPadding, sliderWidth, rightArea.getHeight() - rightSliderTopPadding);
}
