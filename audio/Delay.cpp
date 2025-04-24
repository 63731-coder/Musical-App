//
// Created by Nicoleta on 24-04-25.
//

#include "Delay.h"
#include "../utils/Constants.h"

Delay::Delay() {
    setDelayTime(delayTime); // initialise le buffer
}

void Delay::setDelayTime(float seconds) {
    delayTime = seconds;
    updateBufferSize();
}

void Delay::setFeedback(float fb) {
    if (fb < 0.0f) fb = 0.0f;
    if (fb > 0.95f) fb = 0.95f; // éviter la saturation
    feedback = fb;
}

void Delay::updateBufferSize() {
    delaySamples = static_cast<std::size_t>(delayTime * SAMPLE_RATE);
    bufferSize = delaySamples + 1;
    buffer.resize(bufferSize, 0.0f);
    writeIndex = 0;
}

float Delay::process(float input) {
    std::size_t readIndex = (writeIndex + 1) % bufferSize;
    float delayedSample = buffer[readIndex];

    // Nouveau signal avec retour
    float output = input + delayedSample;
    buffer[writeIndex] = input + delayedSample * feedback;

    // Avancer dans le buffer
    writeIndex = (writeIndex + 1) % bufferSize;

    return output;
}
