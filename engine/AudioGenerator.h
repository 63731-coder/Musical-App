#ifndef AUDIOGENERATOR_H
#define AUDIOGENERATOR_H


#include <memory>

#include "portaudio.h"
#include "../audio/Oscillator.h"
#include "../audio/Filter.h"
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

    void outputToStereo(float *out, const float *monoBuffer); // Convert mono signal to stereo

private:
    static int audioCallback(const void *, void *outputBuffer,
                             unsigned long framesPerBuffer,
                             const PaStreamCallbackTimeInfo *, PaStreamCallbackFlags,
                             void *userData);

    void handleNoteEvents(const SynthParameters &paramsSnapshot);


    PaStream *stream{nullptr}; // PortAudio stream
    LockedSynthParameters &params;

    Oscillator osc1;
    Oscillator osc2;
    Filter filter;
    Delay delay;
    Envelope envelope;

    bool previousNoteState{false}; // Used to detect note on/off changes
};
#endif // AUDIOGENERATOR_H
