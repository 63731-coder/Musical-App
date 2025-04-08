//
// Created by Nicoleta on 03-04-25.
//
#include "LowPassFilter.h"
#include <cmath>

LowPassFilter::LowPassFilter(double sampleRate) : sampleRate(sampleRate), cutoff(1000.0), resonance(0.5) {
    updateFilter();
}

void LowPassFilter::setCutoff(double freq) {
    cutoff = freq;
    updateFilter();
}

void LowPassFilter::setResonance(double res) {
    resonance = res;
    updateFilter();
}

void LowPassFilter::process(std::vector<float>& buffer) {
    for (size_t i = 0; i < buffer.size(); ++i) {
        double input = buffer[i];

        // Filter algorithm
        double output = a0 * input + a1 * x1 + a2 * x2 - b1 * y1 - b2 * y2;

        // Update states
        x2 = x1;
        x1 = input;
        y2 = y1;
        y1 = output;

        buffer[i] = output;
    }
}

void LowPassFilter::updateFilter() {
    q = 0.5 / (1.0 - resonance);
    omega = 2.0 * M_PI * cutoff / sampleRate;
    alpha = std::sin(omega) / (2.0 * q);
    cosw = std::cos(omega);

    norm = 1.0 / (1.0 + alpha);
    a0 = (1.0 - cosw) * 0.5 * norm;
    a1 = (1.0 - cosw) * norm;
    a2 = (1.0 - cosw) * 0.5 * norm;
    b1 = -2.0 * cosw * norm;
    b2 = (1.0 - alpha) * norm;
}
