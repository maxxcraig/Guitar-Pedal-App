#pragma once
#include "BaseEffects.h"

class BluesDriver : public BaseEffect {
public:
    float processSample(float sample) override {
        return sample * (1.0f + drive) / (1.0f + std::abs(sample * drive));
    }

    void setParameter(float value) override {
        drive = value;
    }

    void setSampleRate(double rate) override {}

private:
    float drive = 4.0f;
};
