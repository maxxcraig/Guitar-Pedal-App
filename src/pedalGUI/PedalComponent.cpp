#include "PedalComponent.h"

PedalComponent::PedalComponent(const juce::String& name, std::unique_ptr<BaseEffect> effect, const juce::String& imagePathIn)
    : pedalName(name), imagePath(imagePathIn), effectProcessor(std::move(effect))
{
    auto imageFile = juce::File::getSpecialLocation(juce::File::currentExecutableFile)
                        .getParentDirectory()
                        .getChildFile("resources")
                        .getChildFile(imagePath);

    pedalImage = juce::ImageFileFormat::loadFrom(imageFile);

    if (!pedalImage.isValid())
        juce::Logger::writeToLog("Failed to load image: " + imageFile.getFullPathName());
    else
        juce::Logger::writeToLog("Image loaded: " + imageFile.getFullPathName());
}

void PedalComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::transparentBlack);

    if (pedalImage.isValid())
        g.drawImage(pedalImage, getLocalBounds().toFloat());
    else
    {
        g.setColour(juce::Colours::red);
        g.drawFittedText("Image not found", getLocalBounds(), juce::Justification::centred, 1);
    }
}

void PedalComponent::resized() {}

float PedalComponent::processSample(float sample)
{
    if (effectProcessor)
        return effectProcessor->processSample(sample);
    return sample;
}

void PedalComponent::setSampleRate(double rate)
{
    if (effectProcessor)
        effectProcessor->setSampleRate(rate);
}
