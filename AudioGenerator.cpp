#include <iostream>
#include "AudioGenerator.h"
#include <cmath>
#include "utils/Constants.h"

void AudioGenerator::init(LockedSharedAudioParams* params) {
    sharedParams = params;

    // Initialisation des oscillateurs avec les paramètres initiaux
    auto local = sharedParams->getCopy();
    oscillator1.setWaveform(static_cast<WaveformType>(local.oscillator1WaveForm));
    oscillator2.setWaveform(static_cast<WaveformType>(local.oscillator2WaveForm));
    oscillator1.setFrequency(local.oscillator1Frequency);
    oscillator2.setFrequency(local.oscillator2Frequency);

    PaError errorInit = Pa_Initialize();
    if (errorInit != paNoError) {
        std::cerr << "PortAudio error in Pa_Initialize(): "
                  << Pa_GetErrorText(errorInit) << std::endl;
        return;
    }

    PaError errorStream;
    PaStream* stream;

    errorStream = Pa_OpenDefaultStream(&stream,
                                       0,
                                       2,
                                       paFloat32,
                                       SAMPLE_RATE,
                                       FRAMES_PER_BUFFER,
                                       audioCallback,
                                       this);

    if (errorStream != paNoError) {
        std::cerr << "PortAudio error in Pa_OpenDefaultStream(): "
                  << Pa_GetErrorText(errorStream) << std::endl;
        return;
    }

    errorStream = Pa_StartStream(stream);
    if (errorStream != paNoError) {
        std::cerr << "PortAudio error in Pa_StartStream(): "
                  << Pa_GetErrorText(errorStream) << std::endl;
    }
}

int AudioGenerator::audioCallback(const void* inputBuffer,
                                  void* outputBuffer,
                                  unsigned long framesPerBuffer,
                                  const PaStreamCallbackTimeInfo* timeInfo,
                                  PaStreamCallbackFlags statusFlags,
                                  void* userData) {

    auto* self = static_cast<AudioGenerator*>(userData);
    auto params = self->sharedParams->getCopy();

    // Appliquer les paramètres à chaque frame
    self->oscillator1.setWaveform(static_cast<WaveformType>(params.oscillator1WaveForm));
    self->oscillator2.setWaveform(static_cast<WaveformType>(params.oscillator2WaveForm));
    self->envelope.setAttackTime(params.attack);
    self->envelope.setReleaseTime(params.release);
    self->filter.setCutoff(params.cutoff);
    self->filter.setResonance(params.resonance);
    self->delay.setDelayTime(params.delayTime);

    // 🎵 Gérer la note active
    if (params.notePressed) {
        float freq = 220.0f * std::pow(2.0f, params.noteIndex / 12.0f);
        self->oscillator1.setFrequency(freq);
        self->oscillator2.setFrequency(freq);
        self->envelope.noteOn();
    } else {
        self->envelope.noteOff();
    }

    float* audioBuffer = reinterpret_cast<float*>(outputBuffer);

    for (unsigned long i = 0; i < framesPerBuffer; ++i) {
        float sample = 0.0f;

        if (params.oscillator1Enabled)
            sample += self->oscillator1.getNextSample();
        if (params.oscillator2Enabled)
            sample += self->oscillator2.getNextSample();

        sample *= self->envelope.nextSample();
        sample = self->filter.process(sample);

        float dry = sample;
        float wet = self->delay.process(sample);
        sample = dry * (1.0f - params.delayMix) + wet * params.delayMix;

        audioBuffer[i * 2] = sample;       // Left
        audioBuffer[i * 2 + 1] = sample;   // Right
    }

    return 0;
}
//
// void AudioGenerator::noteOn() {
//     envelope.noteOn(); // Plus utilisé si tout est déclenché via params
// }
//
// void AudioGenerator::noteOff() {
//     envelope.noteOff(); // Plus utilisé
// }
