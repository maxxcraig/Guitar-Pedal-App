#pragma once

#include <JuceHeader.h>
#include "BaseEffects.h"

class PedalComponent : public juce::Component
{
public:
    PedalComponent(const juce::String& name, std::unique_ptr<BaseEffect> effect, const juce::String& imagePath);
    virtual ~PedalComponent() = default;

    void paint(juce::Graphics& g) override;
    virtual void resized() override;

    float processSample(float sample);
    void setSampleRate(double rate);

protected:
    juce::String pedalName;
    juce::String imagePath; // <--- added to fix constructor usage
    std::unique_ptr<BaseEffect> effectProcessor;
    juce::Image pedalImage;
};
