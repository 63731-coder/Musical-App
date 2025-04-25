//
// Created by Nicoleta on 24-04-25.
//

#include "Delay.h"
#include "../utils/Constants.h"

Delay::Delay() {
    setDelayTime(delayTime); // initialise buffer
}

void Delay::setDelayTime(float seconds) {
    delayTime = seconds;
    updateBufferSize();
}

void Delay::setMix(float newMix) {
    if (newMix < 0.0f) newMix = 0.0f;
    if (newMix > 1.0f) newMix = 1.0f;
    mix = newMix;
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

    float output = input + mix * delayedSample;

    // Write in the buffer
    buffer[writeIndex] = input;

    // Advance in the buffer
    writeIndex = (writeIndex + 1) % bufferSize;

    return output;
}
