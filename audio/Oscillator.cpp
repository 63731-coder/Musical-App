//
// Created by Nicoleta on 10-04-25.
//

#include "Oscillator.h"
#include "../utils/Constants.h"
#include <cmath>



Oscillator::Oscillator()
    : currentSampleRate(Constants::SampleRate),
      currentFrequencyHz(0.0),
      phaseRadians(0.0),
      waveformType(Waveform::SINE) {
}

void Oscillator::setFrequency(double newFrequencyHz) {
    currentFrequencyHz = newFrequencyHz;
}

void Oscillator::setWaveform(Waveform newWaveform) {
    waveformType = newWaveform;
}

void Oscillator::resetPhase() {
    phaseRadians = 0.0;
}

void Oscillator::setSampleRate(double newSampleRate) {
    currentSampleRate = newSampleRate;
}

void Oscillator::process(float *audioBuffer) {
    double phaseIncrement = Constants::TWO_PI * currentFrequencyHz / currentSampleRate;

    for (int i = 0; i < Constants::FramesPerBuffer; ++i) {
        float sampleValue = 0.0f;

        switch (waveformType) {
            case Waveform::SINE:
                sampleValue = static_cast<float>(sin(phaseRadians));
            break;

            case Waveform::SQUARE:
                sampleValue = (phaseRadians < M_PI) ? -1.0f : 1.0f;
            break;

            case Waveform::SAW:
                sampleValue = static_cast<float>((2.0 * (phaseRadians / Constants::TWO_PI)) - 1.0);
            break;
        }

        audioBuffer[i] = sampleValue;

        phaseRadians += phaseIncrement;
        if (phaseRadians >= Constants::TWO_PI)
            phaseRadians -= Constants::TWO_PI;
    }
}
