#ifndef AUDIOGENERATOR_H
#define AUDIOGENERATOR_H

#include <memory>

#include "portaudio.h"
#include "../audio/Oscillator.h"
#include "AudioParam.h"
#include "../audio/Delay.h"
#include "../audio/Envelope.h"
#include "../audio/LowPassFilter.h"


class AudioGenerator {
public:
    explicit AudioGenerator(LockedSynthParameters& sharedParams);
    void init();
private:
    static int audioCallback(const void*, void* outputBuffer,
                             unsigned long framesPerBuffer,
                             const PaStreamCallbackTimeInfo*, PaStreamCallbackFlags,
                             void* userData);

    PaStream* stream {nullptr};
    LockedSynthParameters& params;

    Oscillator osc1;
    Oscillator osc2;
    Envelope envelope;
    LowPassFilter filter;
    Delay delay;


    double currentTimeInSeconds {0.0};
    bool previousNoteState {false};
};

#endif // AUDIOGENERATOR_H