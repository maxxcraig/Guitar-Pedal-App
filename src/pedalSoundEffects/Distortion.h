#pragma once
#include "BaseEffects.h"

class Distortion : public BaseEffect {
public:
    float processSample(float sample) override {
        return std::tanh(sample * gain);
    }

    void setParameter(float value) override {
        gain = value;
    }

    void setSampleRate(double rate) override {}

private:
    float gain = 5.0f;
};
