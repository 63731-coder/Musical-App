#include "AudioGenerator.h"
#include "../utils/Constants.h"
#include <iostream>
#include <algorithm>
#include <cmath>


AudioGenerator::AudioGenerator(LockedSynthParameters &sharedParams)
    : params(sharedParams) {
}


void AudioGenerator::init() {
    PaError errorInit = Pa_Initialize();
    if (errorInit != paNoError) {
        std::cerr << "PortAudio error in Pa_Initialize(): " << Pa_GetErrorText(errorInit) << std::endl;
        return;
    }

    PaError errorStream = Pa_OpenDefaultStream(
        &stream,
        0, 2, paFloat32,
        Constants::SampleRate,
        Constants::FramesPerBuffer,
        &AudioGenerator::audioCallback,
        this
    );
    if (errorStream != paNoError) {
        std::cerr << "PortAudio error in Pa_OpenDefaultStream(): " << Pa_GetErrorText(errorStream) << std::endl;
        return;
    }


    errorStream = Pa_StartStream(stream);
    if (errorStream != paNoError) {
        std::cerr << "PortAudio error in Pa_StartStream(): " << Pa_GetErrorText(errorStream) << '\n';
    }
}

int AudioGenerator::audioCallback(const void *, void *outputBuffer,
                                  unsigned long framesPerBuffer,
                                  const PaStreamCallbackTimeInfo *,
                                  PaStreamCallbackFlags,
                                  void *userData) {
    auto *generator = static_cast<AudioGenerator *>(userData);
    float *out = static_cast<float *>(outputBuffer);

    SynthParameters paramsSnapshot = generator->params.getCopy();

    generator->handleNoteStateChange(paramsSnapshot);
    generator->prepareOscillators(paramsSnapshot);
    float mixBuffer[Constants::FramesPerBuffer] = {};
    generator->generateAndMixBuffers(paramsSnapshot, mixBuffer);
    generator->applyEffects(paramsSnapshot, mixBuffer);
    generator->writeToOutput(out, mixBuffer, framesPerBuffer);

    generator->currentTimeInSeconds += framesPerBuffer / static_cast<double>(Constants::SampleRate);

    return paContinue;
}

void AudioGenerator::handleNoteStateChange(const SynthParameters &paramsSnapshot) {
    bool currentNoteState = paramsSnapshot.activeNote;
    if (currentNoteState && !previousNoteState) {
        osc1.resetPhase();
        osc2.resetPhase();
        envelope.noteOn();
    } else if (!currentNoteState && previousNoteState) {
        envelope.noteOff();
    }
    previousNoteState = currentNoteState;
}

void AudioGenerator::prepareOscillators(const SynthParameters &paramsSnapshot) {
    float noteFreq = 220.0f * std::pow(2.0f, static_cast<float>(paramsSnapshot.noteIndex) / 12.0f); //static-cast bcz noteindex is int
    osc1.setFrequency(noteFreq + paramsSnapshot.osc1FrequencyOffsetHz);
    osc1.setWaveform(static_cast<Oscillator::Waveform>(paramsSnapshot.osc1Waveform));

    osc2.setFrequency(noteFreq);
    osc2.setWaveform(Oscillator::Waveform::SAW);
}

void AudioGenerator::generateAndMixBuffers(const SynthParameters &paramsSnapshot, float *mixBuffer) {
    float bufferOsc1[Constants::FramesPerBuffer] = {};
    float bufferOsc2[Constants::FramesPerBuffer] = {};

    if (paramsSnapshot.osc1Active) osc1.process(bufferOsc1);
    if (paramsSnapshot.osc2Active) osc2.process(bufferOsc2);

    for (unsigned long i = 0; i < Constants::FramesPerBuffer; ++i) {
        if (paramsSnapshot.osc1Active && paramsSnapshot.osc2Active)
            mixBuffer[i] = 0.5f * (bufferOsc1[i] + bufferOsc2[i]);
        else if (paramsSnapshot.osc1Active)
            mixBuffer[i] = bufferOsc1[i];
        else if (paramsSnapshot.osc2Active)
            mixBuffer[i] = bufferOsc2[i];
        else
            mixBuffer[i] = 0.0f;
    }
}

void AudioGenerator::applyEffects(const SynthParameters &paramsSnapshot, float *buffer) {
    envelope.setParameters(paramsSnapshot.envelopeAttackSec, paramsSnapshot.envelopeReleaseSec);
    envelope.process(buffer);

    filter.setCutoff(paramsSnapshot.filterCutoffHz);
    filter.setResonance(paramsSnapshot.filterResonance);
    for (unsigned long i = 0; i < Constants::FramesPerBuffer; ++i) {
        buffer[i] = filter.process(buffer[i]);
    }

    delay.setDelayTime(paramsSnapshot.delayTimeSec);
    delay.setMix(paramsSnapshot.delayMix);
    delay.process(buffer);
}

void AudioGenerator::writeToOutput(float *output, const float *buffer, unsigned long frames) {
    for (unsigned long i = 0; i < frames; ++i) {
        output[2 * i] = buffer[i]; // Left
        output[2 * i + 1] = buffer[i]; // Right
    }
}
