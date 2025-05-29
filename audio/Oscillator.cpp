//
// Created by Nicoleta on 10-04-25.
//

#include "Oscillator.h"
#include "../utils/Constants.h"
#include <cmath>


Oscillator::Oscillator()
    : currentSampleRate(Constants::SampleRate),
      currentFrequencyHz(0.0),
      phase_radians(0.0),
      waveformType(Waveform::SINE) {
}

// Set the oscillator frequency (in Hz)
void Oscillator::setFrequency(double newFrequencyHz) {
    currentFrequencyHz = newFrequencyHz;
}

// Set the oscillator waveform type (SINE, SQUARE, or SAW)
void Oscillator::setWaveform(Waveform newWaveform) {
    waveformType = newWaveform;
}

// Fill the audio buffer with generated waveform samples
void Oscillator::process(float *audioBuffer) {
    // Calculate how much the phase increases each sample
    double phaseIncrement = Constants::TWO_PI * currentFrequencyHz / currentSampleRate;

    for (int i = 0; i < Constants::FramesPerBuffer; ++i) {
        float sampleValue = 0.0f;

        // Generate sample based on the selected waveform
        switch (waveformType) {
            case Waveform::SINE:
                sampleValue = static_cast<float>(std::sin(phase_radians) * 0.5);
                break;

            case Waveform::SQUARE:
                sampleValue = (std::sin(phase_radians) >= 0.0) ? 0.5f : -0.5f;
                break;

            case Waveform::SAW:
                sampleValue = static_cast<float>((phase_radians / (Constants::TWO_PI)) - 0.5);
                break;
        }

        audioBuffer[i] = sampleValue; // Write sample to output buffer

        phase_radians += phaseIncrement; // increment phase

        // wrap phase back to 0 if it completes a full cycle (to stay between 0 and 2pi)
        if (phase_radians >= Constants::TWO_PI)
            phase_radians -= Constants::TWO_PI;
    }
}
