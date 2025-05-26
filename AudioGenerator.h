#ifndef SIMPLE_SYNTH_AUDIOGENERATOR_H
#define SIMPLE_SYNTH_AUDIOGENERATOR_H


#include "portaudio.h"
#include "SharedAudioParams.h"
#include "audio/Oscillator.h"
#include "audio/Envelope.h"
#include "audio/LowPassFilter.h"
#include "audio/Delay.h"

class AudioGenerator {
public:
    void init(LockedSharedAudioParams* params);
    void noteOn();
    void noteOff();
    void setEnvelopeParams(float attack, float release);
    void setFilterParams(float cutoff, float resonance);
    void setDelayParams(float timeInSec, float feedback);


private:
    Oscillator oscillator1;
    Oscillator oscillator2;
    Envelope envelope;
    Delay delay;
    LowPassFilter filter;

    LockedSharedAudioParams* sharedParams = nullptr;

    static int audioCallback(const void *inputBuffer, void *outputBuffer,
                             unsigned long framesPerBuffer,
                             const PaStreamCallbackTimeInfo* timeInfo,
                             PaStreamCallbackFlags statusFlags,
                             void *userData);
    double currentTimeInSeconds {0.0};
};

#endif //SIMPLE_SYNTH_AUDIOGENERATOR_H
