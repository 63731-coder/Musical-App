#include <iostream>
#include "AudioGenerator.h"

#include <cmath>

#include "utils/Constants.h"



void AudioGenerator::init() {

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
                                       nullptr );

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

    const int audioBufferSize = framesPerBuffer*2;
    const int frequency = 440;
    float* audioBuffer = reinterpret_cast<float *>(outputBuffer);
    for (int i = 0; i < audioBufferSize; i+=2) {
        audioBuffer[i] = std::sin(TWO_PI * frequency * i / SAMPLE_RATE);
        audioBuffer[i+1] = audioBuffer[i];
    }

    return 0;
}

