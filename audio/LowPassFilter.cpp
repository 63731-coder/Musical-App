//
// Created by Nicoleta on 18-04-25.
//

#include "LowPassFilter.h"
#include <cmath>

#include "../utils/Constants.h"

LowPassFilter::LowPassFilter() {
    updateCoefficients();
}

void LowPassFilter::setCutoff(float cutoffHz) {
    cutoff = cutoffHz;
    updateCoefficients();
}

void LowPassFilter::setResonance(float res) {
    resonance = res;
    updateCoefficients();
}

void LowPassFilter::updateCoefficients() {
    float q = 0.5f / (1.0f - resonance);
    float omega = Constants::TWO_PI * cutoff / Constants::SampleRate;
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
    float output = a0 * input + a1 * x1 + a2 * x2 - b1 * y1 - b2 * y2;

    // Update state
    x2 = x1;
    x1 = input;
    y2 = y1;
    y1 = output;

    return output;
}

