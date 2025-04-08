//
// Created by Nicoleta on 03-04-25.
//

#include "Delay.h"

Delay::Delay(double sampleRate) : sampleRate(sampleRate), delayTime(0.5), delayMix(0.5) {
    buffer.resize(static_cast<size_t>(sampleRate * 2)); // maximum delay of 2 seconds
    readIndex = 0;
    writeIndex = 0;
}

void Delay::setDelayTime(double time) {
    delayTime = time;
    int delaySamples = static_cast<int>(time * sampleRate);
    writeIndex = delaySamples % buffer.size();
    readIndex = (writeIndex + buffer.size() - delaySamples) % buffer.size();
}

void Delay::setDelayMix(double mix) {
    delayMix = mix;
}

void Delay::process(std::vector<float>& bufferInput) {
    for (size_t i = 0; i < bufferInput.size(); ++i) {
        float input = bufferInput[i];
        float output = input + delayMix * buffer[readIndex];

        // Write to buffer
        buffer[writeIndex] = output;

        // Update indices
        writeIndex = (writeIndex + 1) % buffer.size();
        readIndex = (readIndex + 1) % buffer.size();

        bufferInput[i] = output;
    }
}
