//
// Created by Nicoleta on 17-04-25.
//

#include "Envelope.h"
#include "../utils/Constants.h"

Envelope::Envelope() {
    updateIncrements();
}

void Envelope::setAttackTime(float seconds) {
    attackTime = seconds;
    updateIncrements();
}

void Envelope::setReleaseTime(float seconds) {
    releaseTime = seconds;
    updateIncrements();
}

void Envelope::noteOn() {
    state = State::ATTACK;
}

void Envelope::noteOff() {
    state = State::RELEASE;
}

float Envelope::nextSample() {
    switch (state) {
        case State::ATTACK:
            amplitude += attackIncrement;
        if (amplitude >= 1.0f) {
            amplitude = 1.0f;
            state = State::IDLE;
        }
        break;

        case State::RELEASE:
            amplitude -= releaseIncrement;
        if (amplitude <= 0.0f) {
            amplitude = 0.0f;
            state = State::IDLE;
        }
        break;

        case State::IDLE:
            default:
                break;
    }

    return amplitude;
}

void Envelope::updateIncrements() {
    if (attackTime > 0.0f)
        attackIncrement = 1.0f / (attackTime * SAMPLE_RATE);
    else
        attackIncrement = 1.0f;

    if (releaseTime > 0.0f)
        releaseIncrement = 1.0f / (releaseTime * SAMPLE_RATE);
    else
        releaseIncrement = 1.0f;
}
