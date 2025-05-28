#include "AudioGenerator.h"
#include "../utils/Constants.h"
#include <iostream>
#include <cmath>

#include "../audio/Delay.h"

AudioGenerator::AudioGenerator(LockedSynthParameters &sharedParams)
    : params(sharedParams) {
}

void AudioGenerator::handleNoteEvents(const SynthParameters &paramsSnapshot) {
    bool currentNoteState = paramsSnapshot.activeNote;
    if (currentNoteState == true && !previousNoteState) {
        envelope.noteOn();
    } else if (!currentNoteState && previousNoteState) {
        envelope.noteOff();
    }
    previousNoteState = currentNoteState;
}


void AudioGenerator::processOscillators(float *outBuffer, const SynthParameters &params, unsigned long numSamples) {
float noteFreq = 220.0f * std::pow(2.0f, static_cast<float>(params.noteIndex) / 12.0f);
    osc1.setFrequency(noteFreq + params.osc1FrequencyOffsetHz);
    osc1.setWaveform(static_cast<Oscillator::Waveform>(params.osc1Waveform));

    osc2.setFrequency(noteFreq);
    osc2.setWaveform(Oscillator::Waveform::SAW);

    float bufferOsc1[Constants::FramesPerBuffer]{};
    float bufferOsc2[Constants::FramesPerBuffer]{};

    if (params.osc1Active)
        osc1.process(bufferOsc1);
    if (params.osc2Active)
        osc2.process(bufferOsc2);

    for (unsigned long i = 0; i < numSamples; ++i)
        outBuffer[i] = 0.5f * (bufferOsc1[i] + bufferOsc2[i]);
}

void AudioGenerator::applyEnvelope(float *buffer) {
    envelope.process(buffer);
}


void AudioGenerator::applyFilter(float *buffer, const SynthParameters &params) {
    filter.setCutoffFrequencyHz(params.filterCutoffHz);
    filter.setResonance(params.filterResonance);
    for (unsigned long i = 0; i < Constants::FramesPerBuffer; ++i)
        buffer[i] = filter.process(buffer[i]);
}


void AudioGenerator::applyDelay(float *buffer, const SynthParameters &params) {
    delay.setDelayTime(params.delayTimeSec);
    delay.setMix(params.delayMix);
    delay.process(buffer);
}

void AudioGenerator::outputToStereo(float *out, const float *monoBuffer) {
    for (unsigned long i = 0; i < Constants::FramesPerBuffer; ++i) {
        out[2 * i] = monoBuffer[i];
        out[2 * i + 1] = monoBuffer[i];
    }
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

    // Gère les événements de note (note on/off)
    generator->handleNoteEvents(paramsSnapshot);

    // Génère les signaux des oscillateurs
    float mixBuffer[Constants::FramesPerBuffer]{};
    generator->processOscillators(mixBuffer, paramsSnapshot, framesPerBuffer);

    generator->envelope.setParameters(
        paramsSnapshot.envelopeAttackSec,
        paramsSnapshot.envelopeReleaseSec
    );

    // Applique l'enveloppe ADSR
    generator->applyEnvelope(mixBuffer);

    // Applique le filtre passe-bas
    generator->applyFilter(mixBuffer, paramsSnapshot);

    // Applique l'effet de delay
    generator->applyDelay(mixBuffer, paramsSnapshot);

    // Sortie stéréo + enregistrement WAV
    generator->outputToStereo(out, mixBuffer);

    // Mise à jour du temps courant
    generator->currentTimeInSeconds += framesPerBuffer / static_cast<double>(Constants::SampleRate);
    // generator->wavOut.push_frame()

    return paContinue;
}
