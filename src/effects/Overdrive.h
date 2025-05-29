#pragma once
#include "BaseEffects.h"
#include <cmath>

class Overdrive : public BaseEffect {
public:
    float processSample(float input) override;
    void setParameter(float value) override;
    void setGain(float value);

private:
    float gain = 1.0f;
};
