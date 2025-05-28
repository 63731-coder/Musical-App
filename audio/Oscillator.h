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

    explicit Oscillator();

    void setFrequency(double newFrequencyHz);

    void setWaveform(Waveform newWaveform);

    void setSampleRate(double newSampleRate);

    void resetPhase();


    void process(float *audioBuffer);

private:
    double currentSampleRate;
    double currentFrequencyHz;
    double phaseRadians;
    Waveform waveformType;
};


#endif //OSCILLATOR_H
