#pragma once

class BaseEffect {
public:
    virtual ~BaseEffect() = default;
    virtual float processSample(float input) = 0;
    virtual void setParameter(float value) {}
};
