#pragma once

#include "PedalComponent.h"
#include "../pedalSoundEffects/Overdrive.h"

class OverdriveComponent : public PedalComponent
{
public:
    OverdriveComponent();
    void resized() override;
    float processSample(float sample);

private:
    juce::Slider levelKnob, driveKnob, toneKnob;
    float level = 1.0f;
    Overdrive* overdrivePtr = nullptr;
};
