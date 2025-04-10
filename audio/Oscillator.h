//
// Created by Nicoleta on 10-04-25.
//

#ifndef OSCILLATOR_H
#define OSCILLATOR_H



enum class WaveformType {
    SINE,
    SQUARE,
    SAW
};

class Oscillator {
public:
    Oscillator();
    void setFrequency(float freq);
    void setWaveform(WaveformType type);
    float getNextSample();

private:
    float frequency;
    WaveformType waveform;
    double phase;               // Current position within the waveform cycle (between 0 and 2π)
    double phaseIncrement;      // Amount by which the phase advances at each audio tick
    void updatePhaseIncrement();// Recomputes phase increment when frequency changes
};



#endif //OSCILLATOR_H
