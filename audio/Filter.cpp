//
// Created by Nicoleta on 19-04-25.
//
#include "Filter.h"
#include <cmath>
#include "../utils/Constants.h"

Filter::Filter() : cutoffFrequencyHz(15000.0f), resonance(0.5f) {
    updateCoefficients(); // calculate filter math based on default settings
}

void Filter::setCutoffFrequencyHz(const float newCutoffFrequencyHz) {
    cutoffFrequencyHz = newCutoffFrequencyHz;
    updateCoefficients(); // recalculate with the new frequency
}


void Filter::setResonance(const float newResonance) {
    resonance = newResonance;
    updateCoefficients(); // recalculate with the new resonance
}

void Filter::updateCoefficients() {
    const float q = 0.5f / (1.0f - resonance);
    const float omega = (Constants::TWO_PI) * cutoffFrequencyHz / Constants::SampleRate;
    const float alpha = std::sin(omega) / (2.0f * q);
    const float cosw = std::cos(omega);
    const float norm = 1.0f / (1.0f + alpha);

    a0 = (1.0f - cosw) * 0.5f * norm;
    a1 = (1.0f - cosw) * norm;
    a2 = a0;
    b1 = -2.0f * cosw * norm;
    b2 = (1.0f - alpha) * norm;
}


float Filter::process(const float input) {
    // Calculate the output using current and past values
    float output = a0 * input + a1 * prevInput1 + a2 * prevInput2 - b1 * prevOutput1 - b2 * prevOutput2;

    // Save the current input and output to use in the next call
    prevInput2 = prevInput1;
    prevInput1 = input;
    prevOutput2 = prevOutput1;
    prevOutput1 = output;

    return output; // return the filtered sample
}
