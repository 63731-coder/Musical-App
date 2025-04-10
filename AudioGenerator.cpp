#include <iostream>
#include "AudioGenerator.h"

#include <cmath>

#include "utils/Constants.h"



void AudioGenerator::init(Oscillator* osc) {
    static AudioCallbackData callbackData;  // notre structure créée à l’étape 1
    callbackData.osc = osc;                 // on lui donne un pointeur vers l’oscillateur

    PaError errorInit = Pa_Initialize();
    if( errorInit != paNoError ) {
        std::cerr << "PortAudio error in Pa_Initialize(): "
                  << Pa_GetErrorText( errorInit ) << std::endl;
        return;
    }

    PaError errorStream;
    PaStream *stream;

    errorStream = Pa_OpenDefaultStream(&stream,
                                       0,
                                       2,
                                       paFloat32,
                                       SAMPLE_RATE,
                                       FRAMES_PER_BUFFER,
                                       audioCallback,
                                       &callbackData); // <-- pass the structure from .h


    errorStream = Pa_StartStream( stream );
    if( errorStream != paNoError ) {
        std::cerr << "PortAudio error in Pa_StartStream(): "
                  << Pa_GetErrorText( errorStream ) << std::endl;
    }
}

int AudioGenerator::audioCallback(const void *inputBuffer,
                                  void *outputBuffer,
                                  unsigned long framesPerBuffer,
                                  const PaStreamCallbackTimeInfo *timeInfo,
                                  PaStreamCallbackFlags statusFlags,
                                  void *userData) {

    auto* data = static_cast<AudioCallbackData*>(userData);
    Oscillator* osc = data->osc;
    float* audioBuffer = reinterpret_cast<float *>(outputBuffer);

    for (unsigned long i = 0; i < framesPerBuffer; ++i) {
        float sample = osc->getNextSample();
        audioBuffer[i * 2] = sample;
        audioBuffer[i * 2 + 1] = sample;
    }

    return 0;
}

