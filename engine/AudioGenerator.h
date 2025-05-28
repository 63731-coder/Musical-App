#ifndef AUDIOGENERATOR_H
#define AUDIOGENERATOR_H


#include <memory>

#include "portaudio.h"
#include "../audio/Oscillator.h"
#include "../audio/LowPassFilter.h"
#include "AudioParam.h"
#include "../audio/Delay.h"
#include "../audio/Envelope.h"


class AudioGenerator {
public:
    explicit AudioGenerator(LockedSynthParameters &sharedParams);

    void init();

    void processOscillators(float *outBuffer, const SynthParameters &params, unsigned long numSamples);

    void applyEnvelope(float *buffer);

    void applyFilter(float *buffer, const SynthParameters &params);

    void applyDelay(float *buffer, const SynthParameters &params);

    void outputToStereo(float *out, const float *monoBuffer);

private:
    static int audioCallback(const void *, void *outputBuffer,
                             unsigned long framesPerBuffer,
                             const PaStreamCallbackTimeInfo *, PaStreamCallbackFlags,
                             void *userData);

    void handleNoteEvents(const SynthParameters &paramsSnapshot);


    PaStream *stream{nullptr};
    LockedSynthParameters &params;

    Oscillator osc1;
    Oscillator osc2;
    LowPassFilter filter;
    Delay delay;
    Envelope envelope;


    double currentTimeInSeconds{0.0};
    bool previousNoteState{false};
};
#endif // AUDIOGENERATOR_H
