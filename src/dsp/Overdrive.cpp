#include "Overdrive.h"

float Overdrive::processSample(float input) {
    float drive = gain * input;
    return std::tanh(drive);
}

void Overdrive::setParameter(float value) {
    setGain(value);
}

void Overdrive::setGain(float value) {
    gain = value;
}
