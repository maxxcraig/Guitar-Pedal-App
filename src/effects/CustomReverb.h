#pragma once
#include "BaseEffects.h"
#include <JuceHeader.h>

class CustomReverb : public BaseEffect {
public:
    float processSample(float input) override;
    void setParameter(float value) override;
    void setMix(float value);
    void setSampleRate(double rate);

private:
    juce::Reverb reverb;
    juce::Reverb::Parameters params;
};
