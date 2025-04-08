//
// Created by Nicoleta on 03-04-25.
//

#include "Oscillator.h"
#include <cmath>

Oscillator::Oscillator(double sampleRate) : sampleRate(sampleRate), phase(0.0), frequency(440.0), waveform(Waveform::Sine) {}

void Oscillator::setFrequency(double freq) {
    frequency = freq;
    phaseStep = 2.0 * M_PI * frequency / sampleRate;
}

void Oscillator::setWaveform(Waveform wf) {
    waveform = wf;
}

void Oscillator::generate(std::vector<float>& buffer) {
    for (size_t i = 0; i < buffer.size(); ++i) {
        switch (waveform) {
            case Waveform::Sine:
                buffer[i] = std::sin(phase);
            break;
            case Waveform::Square:
                buffer[i] = (std::sin(phase) >= 0) ? 0.5f : -0.5f;
            break;
            case Waveform::Saw:
                buffer[i] = (2.0f * (phase / (2.0 * M_PI)) - 1.0f);
            break;
        }

        phase += phaseStep;
        if (phase >= 2.0 * M_PI) {
            phase -= 2.0 * M_PI;
        }
    }
}
