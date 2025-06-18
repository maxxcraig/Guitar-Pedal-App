#pragma once

#include <JuceHeader.h>
#include "BaseEffects.h"

class PedalComponent : public juce::Component,
                       public juce::Button::Listener,
                       public juce::Slider::Listener
{
public:
    PedalComponent(const juce::String& name, std::unique_ptr<BaseEffect> effect);
    void resized() override;
    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;

    float processSample(float sample);
    void setSampleRate(double rate);

private:
    juce::String pedalName;
    bool isActive = true;

    std::unique_ptr<BaseEffect> effectProcessor;

    juce::ToggleButton toggleButton;
    juce::Slider controlSlider;
    juce::Label sliderLabel;
};
