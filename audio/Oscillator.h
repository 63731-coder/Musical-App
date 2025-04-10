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
    double phase;
    double phaseIncrement;
    void updatePhaseIncrement();
};



#endif //OSCILLATOR_H
