//
// Created by Nicoleta on 18-04-25.
//
#include "LowPassFilter.h"
#include <cmath>
#include "../utils/Constants.h"

LowPassFilter::LowPassFilter() {
    updateCoefficients();
}

void LowPassFilter::setCutoffFrequencyHz(float newCutoffFrequencyHz) {
    cutoffFrequencyHz = newCutoffFrequencyHz;
    updateCoefficients();
}


void LowPassFilter::setResonance(float newResonance) {
    resonance = newResonance;
    updateCoefficients();
}

void LowPassFilter::updateCoefficients() {
    float q = 0.5f / (1.0f - resonance);
    float omega = (Constants::TWO_PI) * cutoffFrequencyHz / Constants::SampleRate;
    float alpha = std::sin(omega) / (2.0f * q);
    float cosw = std::cos(omega);
    float norm = 1.0f / (1.0f + alpha);

    a0 = (1.0f - cosw) * 0.5f * norm;
    a1 = (1.0f - cosw) * norm;
    a2 = (1.0f - cosw) * 0.5f * norm;
    b1 = -2.0f * cosw * norm;
    b2 = (1.0f - alpha) * norm;
}


float LowPassFilter::process(float input) {
    float output = a0 * input + a1 * prevInput1 + a2 * prevInput2 - b1 * prevOutput1 - b2 * prevOutput2;

    prevInput2 = prevInput1;
    prevInput1 = input;
    prevOutput2 = prevOutput1;
    prevOutput1 = output;
    return output;
}
