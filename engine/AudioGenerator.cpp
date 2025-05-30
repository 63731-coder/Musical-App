#include "AudioGenerator.h"
#include "../utils/Constants.h"
#include <iostream>
#include <cmath>

#include "../audio/Delay.h"

// Constructor: Initializes the AudioGenerator with shared thread-safe parameters
AudioGenerator::AudioGenerator(LockedSynthParameters &sharedParams)
    : params(sharedParams) {
}

// Handles note on/off state transitions and triggers envelope accordingly
void AudioGenerator::handleNoteEvents(const SynthParameters &paramsSnapshot) {
    const bool currentNoteState = paramsSnapshot.activeNote;
    if (currentNoteState == true && !previousNoteState) {
        envelope.noteOn(); // Note pressed
    } else if (!currentNoteState && previousNoteState) {
        envelope.noteOff(); // Note released
    }
    previousNoteState = currentNoteState;
}

// Generates oscillator signals and mixes them into outBuffer
void AudioGenerator::processOscillators(float *outBuffer, const SynthParameters &params,
                                        const unsigned long numSamples) {
    // Convert note index to frequency in Hz
    const float noteFreq = 220.0f * std::pow(2.0f, static_cast<float>(params.noteIndex) / 12.0f);

    osc1.setFrequency(noteFreq + params.osc1FrequencyOffsetHz);
    osc1.setWaveform(static_cast<Oscillator::Waveform>(params.osc1Waveform));

    osc2.setFrequency(noteFreq);
    osc2.setWaveform(Oscillator::Waveform::SAW);

    float bufferOsc1[Constants::FramesPerBuffer]{};
    float bufferOsc2[Constants::FramesPerBuffer]{};

    // Process each oscillator if active
    if (params.osc1Active)
        osc1.process(bufferOsc1);
    if (params.osc2Active)
        osc2.process(bufferOsc2);

    // Mix both oscillators into the output buffer
    for (unsigned long i = 0; i < numSamples; ++i)
        outBuffer[i] = 0.5f * (bufferOsc1[i] + bufferOsc2[i]);
}

// Applies the ADSR envelope to the buffer
void AudioGenerator::applyEnvelope(float *buffer) {
    envelope.process(buffer);
}

// Applies the filter to the buffer using current parameters
void AudioGenerator::applyFilter(float *buffer, const SynthParameters &params) {
    filter.setCutoffFrequencyHz(params.filterCutoffHz);
    filter.setResonance(params.filterResonance);
    for (unsigned long i = 0; i < Constants::FramesPerBuffer; ++i)
        buffer[i] = filter.process(buffer[i]);
}

// Applies a delay effect to the buffer
void AudioGenerator::applyDelay(float *buffer, const SynthParameters &params) {
    delay.setDelayTime(params.delayTimeSec);
    delay.setMix(params.delayMix);
    delay.process(buffer);
}

// Converts mono buffer to stereo by copying the signal to both left and right channels
void AudioGenerator::outputToStereo(float *out, const float *monoBuffer) {
    for (unsigned long i = 0; i < Constants::FramesPerBuffer; ++i) {
        out[2 * i] = monoBuffer[i]; //left channel
        out[2 * i + 1] = monoBuffer[i]; //right channel
    }
}

// Initializes PortAudio and starts the audio stream
void AudioGenerator::init() {
    PaError errorInit = Pa_Initialize();
    if (errorInit != paNoError) {
        std::cerr << "PortAudio error in Pa_Initialize(): " << Pa_GetErrorText(errorInit) << std::endl;
        return;
    }
    PaError errorStream = Pa_OpenDefaultStream(
        &stream,
        0, 2, paFloat32, // No input, 2 output channels, 32-bit float
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

// Main audio processing callback for PortAudio
int AudioGenerator::audioCallback(const void *, void *outputBuffer,
                                  unsigned long framesPerBuffer,
                                  const PaStreamCallbackTimeInfo *,
                                  PaStreamCallbackFlags,
                                  void *userData) {
    auto *generator = static_cast<AudioGenerator *>(userData);
    auto *out = static_cast<float *>(outputBuffer);

    // Copy synth parameters (thread-safe)
    SynthParameters paramsSnapshot = generator->params.getCopy();

    generator->handleNoteEvents(paramsSnapshot); // (note on/off)

    // Generate oscillator output
    float mixBuffer[Constants::FramesPerBuffer]{};
    generator->processOscillators(mixBuffer, paramsSnapshot, framesPerBuffer);

    generator->envelope.setParameters(
        paramsSnapshot.envelopeAttackSec,
        paramsSnapshot.envelopeReleaseSec
    );

    generator->applyEnvelope(mixBuffer);

    generator->applyFilter(mixBuffer, paramsSnapshot);

    generator->applyDelay(mixBuffer, paramsSnapshot);

    generator->outputToStereo(out, mixBuffer);

    return paContinue;
}
