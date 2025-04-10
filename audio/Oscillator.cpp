//
// Created by Nicoleta on 10-04-25.
//

// src/audio/Oscillator.cpp
#include "Oscillator.h"
#include "../utils/Constants.h"
#include <cmath>
#include <iostream>
#include <ostream>

Oscillator::Oscillator()
    : frequency(440.0f), waveform(WaveformType::SINE), phase(0.0)
{
    updatePhaseIncrement();
}

void Oscillator::setFrequency(float freq) {
    frequency = freq;
    updatePhaseIncrement();
}

void Oscillator::setWaveform(WaveformType type) {
    waveform = type;
}

void Oscillator::updatePhaseIncrement() {
    phaseIncrement = TWO_PI * frequency / SAMPLE_RATE;
}

float Oscillator::getNextSample() {
    float sample = 0.0f;
    switch (waveform) {
        case WaveformType::SINE:
            sample = static_cast<float>(std::sin(phase));
        break;
        case WaveformType::SQUARE:
            sample = (std::sin(phase) >= 0.0) ? 0.5f : -0.5f;
        break;
        case WaveformType::SAW:
            sample = static_cast<float>(2.0 * (phase / (TWO_PI)) - 1.0);
        break;
    }

    phase += phaseIncrement;
    if (phase >= TWO_PI) {
        phase -= TWO_PI;
    }
    return sample * 0.3f; // lower volume
}
