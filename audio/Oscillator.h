//
// Created by Nicoleta on 10-04-25.
//

#ifndef OSCILLATOR_H
#define OSCILLATOR_H

class Oscillator {
public:
    enum class Waveform {
        SINE,
        SQUARE,
        SAW
    };

    Oscillator();

    void setFrequency(double newFrequencyHz);

    void setWaveform(Waveform newWaveform);

    void process(float *audioBuffer);

private:
    double currentSampleRate;
    double currentFrequencyHz;
    double phase_radians;
    Waveform waveformType;
};


#endif //OSCILLATOR_H
