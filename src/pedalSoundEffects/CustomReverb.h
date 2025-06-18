#pragma once

#include "BaseEffects.h"
#include <JuceHeader.h>

class CustomReverb : public BaseEffect {
public:
    CustomReverb();
    void setMix(float value);
    float processSample(float input) override;
    void setParameter(float value) override;
    void setSampleRate(double rate) override;

private:
    juce::Reverb reverb;
    juce::Reverb::Parameters params;
};
