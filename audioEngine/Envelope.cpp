//
// Created by Nicoleta on 03-04-25.
//
#include "Envelope.h"
#include <cmath>

Envelope::Envelope(double sampleRate) : sampleRate(sampleRate), currentTime(0.0), currentPhase(Phase::Idle), attackTime(0.0), releaseTime(0.0), sustainLevel(0.0), decayTime(0.0), currentLevel(0.0) {}

void Envelope::setAttack(double time) {
    attackTime = time;
}

void Envelope::setDecay(double time) {
    decayTime = time;
}

void Envelope::setSustain(double level) {
    sustainLevel = level;
}

void Envelope::setRelease(double time) {
    releaseTime = time;
}

void Envelope::noteOn() {
    currentPhase = Phase::Attack;
    currentTime = 0.0;
}

void Envelope::noteOff() {
    currentPhase = Phase::Release;
    currentTime = 0.0;
}

void Envelope::process(std::vector<float>& buffer) {
    for (size_t i = 0; i < buffer.size(); ++i) {
        switch (currentPhase) {
            case Phase::Attack:
                currentLevel += 1.0 / (attackTime * sampleRate);
            if (currentLevel >= 1.0) {
                currentLevel = 1.0;
                currentPhase = Phase::Decay;
            }
            break;

            case Phase::Decay:
                currentLevel -= (1.0 - sustainLevel) / (decayTime * sampleRate);
            if (currentLevel <= sustainLevel) {
                currentLevel = sustainLevel;
                currentPhase = Phase::Sustain;
            }
            break;

            case Phase::Sustain:
                // Maintain sustain level
                    break;

            case Phase::Release:
                currentLevel -= sustainLevel / (releaseTime * sampleRate);
            if (currentLevel <= 0.0) {
                currentLevel = 0.0;
                currentPhase = Phase::Idle;
            }
            break;

            case Phase::Idle:
                currentLevel = 0.0;
            break;
        }

        buffer[i] *= currentLevel;
    }
}