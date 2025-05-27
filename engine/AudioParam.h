// Created by Nicoleta on 25-04-25.

#ifndef AUDIOPARAM_H
#define AUDIOPARAM_H

#include <mutex>

struct SynthParameters {

    bool osc1Active = true;
    int osc1Waveform = 0;   // 0 sine, 1 square, 2 saw
    float osc1Phase = 0.0f;
    float osc1FrequencyOffsetHz = 0.0f;

    bool osc2Active = true;
    int osc2Waveform = 2;
    float osc2Phase = 0.0f;

    float envelopeAttackSec = 0.1f;
    float envelopeReleaseSec = 0.5f;

    float filterCutoffHz = 15000.0f;
    float filterResonance = 0.5f;

    float delayTimeSec = 0.1f;
    float delayMix = 0.0f;

    bool activeNote = false;
    int noteIndex = 0;
};

class LockedSynthParameters {
public:
    LockedSynthParameters() = default;
    SynthParameters getCopy() const;
    void setCopy(const SynthParameters& newData);

private:
    mutable std::mutex mutex;
    SynthParameters data;
};

#endif // AUDIOPARAM_H
