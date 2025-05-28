#ifndef AUDIOGENERATOR_H
#define AUDIOGENERATOR_H

#include <memory>

#include "portaudio.h"
#include "../audio/Oscillator.h"
#include "AudioParam.h"
#include "../audio/Delay.h"
#include "../audio/Envelope.h"
#include "../audio/LowPassFilter.h"

// Responsible for generating audio output using PortAudio
class AudioGenerator {
public:
    explicit AudioGenerator(LockedSynthParameters &sharedParams);

    void init();

private:
    static int audioCallback(const void *, void *outputBuffer,
                             unsigned long framesPerBuffer,
                             const PaStreamCallbackTimeInfo *, PaStreamCallbackFlags,
                             void *userData);

    PaStream *stream{nullptr};
    LockedSynthParameters &params;

    Oscillator osc1;
    Oscillator osc2;
    Envelope envelope;
    LowPassFilter filter;
    Delay delay;

    double currentTimeInSeconds{0.0};
    bool previousNoteState{false};

    void handleNoteStateChange(const SynthParameters& paramsSnapshot);
    void prepareOscillators(const SynthParameters &paramsSnapshot);
    void generateAndMixBuffers(const SynthParameters& paramsSnapshot, float* mixBuffer);
    void applyEffects(const SynthParameters& paramsSnapshot, float* buffer);
    void writeToOutput(float* output, const float* buffer, unsigned long frames);

};

#endif // AUDIOGENERATOR_H
