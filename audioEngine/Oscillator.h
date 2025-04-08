//
// Created by Nicoleta on 03-04-25.
//

#ifndef OSCILLATOR_H
#define OSCILLATOR_H

#include <vector>

class Oscillator {
public:
    enum class Waveform {
        Sine,
        Square,
        Saw
    };

    Oscillator(double sampleRate);
    void setFrequency(double freq);
    void setWaveform(Waveform wf);
    void generate(std::vector<float>& buffer);

private:
    double sampleRate;
    double frequency;
    double phase;
    double phaseStep;
    Waveform waveform;
};


#endif //OSCILLATOR_H
