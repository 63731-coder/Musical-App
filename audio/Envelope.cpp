//
// Created by Nicoleta on 17-04-25.
//

#include "Envelope.h"
#include <algorithm> // Pour std::clamp
#include "../utils/Constants.h"

Envelope::Envelope(double initialSampleRate)
    : sampleRate(initialSampleRate),
      currentState(State::IDLE),
      envelopeValue(0.0),
      attackDurationSeconds(0.1),
      releaseDurationSeconds(0.5),
      elapsedSamplesInStage(0),
      totalSamplesInStage(0)
{
}

void Envelope::setSampleRate(double newSampleRate) {
    sampleRate = newSampleRate;
}

void Envelope::setParameters(double attackTimeSeconds, double releaseTimeSeconds) {
    attackDurationSeconds = attackTimeSeconds;
    releaseDurationSeconds = releaseTimeSeconds;
}

void Envelope::noteOn() {
    enterState(State::ATTACK);
}

void Envelope::noteOff() {
    if (currentState == State::ATTACK || currentState == State::SUSTAIN) {
        enterState(State::RELEASE);
    }
}

bool Envelope::isRunning() const {
    return currentState != State::IDLE;
}

void Envelope::enterState(State newState) {
    currentState = newState;
    elapsedSamplesInStage = 0;

    switch (currentState) {
        case State::ATTACK:
            totalSamplesInStage = static_cast<int>(attackDurationSeconds * sampleRate);
            break;

        case State::RELEASE:
            totalSamplesInStage = static_cast<int>(releaseDurationSeconds * sampleRate);
            break;

        case State::SUSTAIN:
        case State::IDLE:
            totalSamplesInStage = 0;
            break;
    }
}

void Envelope::process(float* audioBuffer) {
    for (int i = 0; i < Constants::FramesPerBuffer; ++i) {
        switch (currentState) {
            case State::ATTACK:
                if (totalSamplesInStage > 0) {
                    envelopeValue = static_cast<double>(elapsedSamplesInStage) / totalSamplesInStage;
                } else {
                    envelopeValue = 1.0;
                }

                elapsedSamplesInStage++;
                if (elapsedSamplesInStage >= totalSamplesInStage) {
                    enterState(State::SUSTAIN);
                }
                break;

            case State::SUSTAIN:
                envelopeValue = 1.0;
                break;

            case State::RELEASE:
                if (totalSamplesInStage > 0) {
                    envelopeValue = 1.0 - (static_cast<double>(elapsedSamplesInStage) / totalSamplesInStage);
                } else {
                    envelopeValue = 0.0;
                }

                elapsedSamplesInStage++;
                if (elapsedSamplesInStage >= totalSamplesInStage) {
                    enterState(State::IDLE);
                }
                break;

            case State::IDLE:
                envelopeValue = 0.0;
                break;
        }

        envelopeValue = std::clamp(envelopeValue, 0.0, 1.0);
        audioBuffer[i] *= static_cast<float>(envelopeValue);
    }
}


