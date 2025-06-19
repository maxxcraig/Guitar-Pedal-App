#pragma once
#include "BaseEffects.h"
#include <cmath>

class Chorus : public BaseEffect {
public:
    float processSample(float sample) override {
        float mod = std::sin(phase) * depth;
        phase += rate * 2.0f * juce::MathConstants<float>::pi / sampleRate;
        if (phase > juce::MathConstants<float>::twoPi)
            phase -= juce::MathConstants<float>::twoPi;

        return sample * (1.0f - mix) + sample * (1.0f + mod) * mix;
    }

    void setParameter(float value) override {
        depth = value / 10.0f;
    }

    void setSampleRate(double rateIn) override {
        sampleRate = rateIn;
    }

private:
    float depth = 0.5f;
    float rate = 0.25f;
    float phase = 0.0f;
    double sampleRate = 44100.0;
    float mix = 0.5f;
};
