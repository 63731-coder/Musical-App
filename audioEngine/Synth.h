//
// Created by Nicoleta on 03-04-25.
//

#ifndef SYNTH_H
#define SYNTH_H

#include <vector>
#include "Oscillator.h"
#include "Envelope.h"
#include "LowPassFilter.h"
#include "Delay.h"



class Synth {
public:
    Synth(double sampleRate);
    void setOscillatorFrequency(double freq);
    void setOscillatorWaveform(Oscillator::Waveform waveform);
    void setEnvelopeAttack(double attackTime);
    void setEnvelopeRelease(double releaseTime);
    void setFilterCutoff(double cutoff);
    void setFilterResonance(double resonance);
    void setDelayTime(double delayTime);
    void setDelayMix(double delayMix);
    void processAudio(std::vector<float>& buffer);
    void noteOn();
    void noteOff();

private:
    double sampleRate;
    Oscillator oscillator1;
    Oscillator oscillator2;
    Envelope envelope;
    LowPassFilter filter;
    Delay delay;
};



#endif //SYNTH_H
