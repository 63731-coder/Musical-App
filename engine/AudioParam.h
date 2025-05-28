// Created by Nicoleta on 25-04-25.

#ifndef AUDIOPARAM_H
#define AUDIOPARAM_H

#include <mutex>

struct SynthParameters {

    // Oscillator 1
    bool osc1Active = true;
    int osc1Waveform = 0;   // 0 sine, 1 square, 2 saw
    float osc1Phase = 0.0f;
    float osc1FrequencyOffsetHz = 0.0f;

    // Oscillator 2
    bool osc2Active = true;
    float osc2Phase = 0.0f;

    // Envelope
    float envelopeAttackSec = 0.1f;
    float envelopeReleaseSec = 0.5f;

    // Filter
    float filterCutoffHz = 15000.0f;
    float filterResonance = 0.5f;

    // Delay
    float delayTimeSec = 0.1f;
    float delayMix = 0.0f;

    // Note state
    bool activeNote = false;
    int noteIndex = 0;	// 0 to 11 (for calculating frequency)
};

class LockedSynthParameters {
public:
    LockedSynthParameters() = default;
    SynthParameters getCopy() const;
    void upDate(const SynthParameters &newData);

private:
    mutable std::mutex mutex;
    SynthParameters data;
};

#endif // AUDIOPARAM_H
