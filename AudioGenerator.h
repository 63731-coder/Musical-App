#ifndef SIMPLE_SYNTH_AUDIOGENERATOR_H
#define SIMPLE_SYNTH_AUDIOGENERATOR_H

#include "portaudio.h"
#include "audio/Oscillator.h"
#include "audio/Envelope.h"
#include "audio/LowPassFilter.h"
#include "audio/Delay.h"



struct AudioCallbackData {
    Oscillator* osc1 = nullptr;
    Oscillator* osc2 = nullptr;
    Envelope* envelope = nullptr;
    LowPassFilter* filter = nullptr;
    Delay* delay = nullptr;
    float delayMix = 0.5f;

    bool osc1Active = true;
    bool osc2Active = false;
};

class AudioGenerator {
public:
    void init(Oscillator* osc1, Oscillator* osc2);
    AudioCallbackData* getCallbackData();

    void noteOn();
    void noteOff();
    void setEnvelopeParams(float attack, float release);
    void setFilterParams(float cutoff, float resonance);
    void setDelayParams(float timeInSec, float feedback);


private:
    Envelope envelope;
    Delay delay;
    LowPassFilter filter; //used at every audio sample to filter the sound
    static int audioCallback(const void *inputBuffer, void *outputBuffer,
                             unsigned long framesPerBuffer,
                             const PaStreamCallbackTimeInfo* timeInfo,
                             PaStreamCallbackFlags statusFlags,
                             void *userData);
    AudioCallbackData callbackData;
    double currentTimeInSeconds {0.0};
};

#endif //SIMPLE_SYNTH_AUDIOGENERATOR_H
