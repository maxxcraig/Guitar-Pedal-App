#include "CustomReverb.h"

float CustomReverb::processSample(float input) {
    float in[] = { input };
    float out[] = { 0.0f };
    reverb.processMono(in, 1);
    return in[0] * (1.0f - params.wetLevel) + in[0] * params.wetLevel;
}

void CustomReverb::setParameter(float value) {
    setMix(value);
}

void CustomReverb::setMix(float value) {
    params.wetLevel = value;
    reverb.setParameters(params);
}

void CustomReverb::setSampleRate(double rate) {
    reverb.setSampleRate(rate);
}
