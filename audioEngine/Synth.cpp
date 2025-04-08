//
// Created by Nicoleta on 03-04-25.
//

#include "Synth.h"

Synth::Synth(double sampleRate)
    : sampleRate(sampleRate),
      oscillator1(sampleRate),
      oscillator2(sampleRate),
      envelope(sampleRate),
      filter(sampleRate),
      delay(sampleRate) {}

void Synth::setOscillatorFrequency(double freq) {
    oscillator1.setFrequency(freq);
    oscillator2.setFrequency(freq);  // Two oscilators qui the same frecquency
}

void Synth::setOscillatorWaveform(Oscillator::Waveform waveform) {
    oscillator1.setWaveform(waveform);
    oscillator2.setWaveform(Oscillator::Waveform::Saw);  // Second oscillator fixed to saw
}

void Synth::setEnvelopeAttack(double attackTime) {
    envelope.setAttack(attackTime);
}

void Synth::setEnvelopeRelease(double releaseTime) {
    envelope.setRelease(releaseTime);
}

void Synth::setFilterCutoff(double cutoff) {
    filter.setCutoff(cutoff);
}

void Synth::setFilterResonance(double resonance) {
    filter.setResonance(resonance);
}

void Synth::setDelayTime(double delayTime) {
    delay.setDelayTime(delayTime);
}

void Synth::setDelayMix(double delayMix) {
    delay.setDelayMix(delayMix);
}

void Synth::processAudio(std::vector<float>& buffer) {
    // Generate the waves of the oscillators
    oscillator1.generate(buffer);
    oscillator2.generate(buffer);

    // Apply envelope
    envelope.process(buffer);

    // Apply filter
    filter.process(buffer);

    // Apply delay
    delay.process(buffer);
}

void Synth::noteOn() {
    envelope.noteOn();
}

void Synth::noteOff() {
    envelope.noteOff();
}
