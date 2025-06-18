#include "PedalComponent.h"

PedalComponent::PedalComponent(const juce::String& name, std::unique_ptr<BaseEffect> effect)
    : pedalName(name), effectProcessor(std::move(effect))
{
    addAndMakeVisible(toggleButton);
    toggleButton.setButtonText(name);
    toggleButton.setToggleState(true, juce::dontSendNotification);
    toggleButton.addListener(this);

    addAndMakeVisible(controlSlider);
    controlSlider.setRange(0.0, 10.0, 0.1);
    controlSlider.setValue(5.0);
    controlSlider.addListener(this);
    controlSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);

    addAndMakeVisible(sliderLabel);
    sliderLabel.setText(name + " Level", juce::dontSendNotification);
    sliderLabel.attachToComponent(&controlSlider, false);
}

void PedalComponent::resized()
{
    int padding = 10;
    int toggleHeight = 30;
    int sliderHeight = 60;

    toggleButton.setBounds(padding, padding, getWidth() - 2 * padding, toggleHeight);
    controlSlider.setBounds(padding, padding + toggleHeight + 10, getWidth() - 2 * padding, sliderHeight);
}

void PedalComponent::buttonClicked(juce::Button* button)
{
    if (button == &toggleButton)
        isActive = toggleButton.getToggleState();
}

void PedalComponent::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &controlSlider && effectProcessor)
        effectProcessor->setParameter(controlSlider.getValue());
}

float PedalComponent::processSample(float sample)
{
    if (isActive && effectProcessor)
        return effectProcessor->processSample(sample);
    return sample;
}

void PedalComponent::setSampleRate(double rate)
{
    if (effectProcessor)
        effectProcessor->setSampleRate(rate);
}
