//
// Created by Nicoleta on 24-04-25.
//

#include "Delay.h"
#include "../utils/Constants.h"

Delay::Delay()
    : writeIndex(0),
      delayTime(0.3f),
      mix(0.2f)
{
    // Initialize the delay buffer with 2 seconds worth of samples
    delayBuffer.resize(Constants::SampleRate * 2, 0.0f);
}

void Delay::setDelayTime(float seconds) {
    delayTime = seconds;
}

void Delay::setMix(float mixValue) {
    mix = mixValue;
}

// Calculate the number of samples to delay based on delayTime
void Delay::process(float *buffer) {
    int delaySamples = static_cast<int>(delayTime * Constants::SampleRate);

    for (int i = 0; i < Constants::FramesPerBuffer; ++i) {
        // Compute the index to read the delayed sample from
        int readIndex = writeIndex - delaySamples;
        if (readIndex < 0) {
            // Wrap around the circular buffer
            readIndex += static_cast<int>(delayBuffer.size());
        }

        float delayedSample = delayBuffer[readIndex];

        // Mix the delayed sample with the original sample
        buffer[i] = buffer[i] + mix * delayedSample;

        // Store the new sample into the buffer (after mixing)
        delayBuffer[writeIndex] = buffer[i];

        // Move to the next index, wrapping around if needed
        writeIndex = (writeIndex + 1) % static_cast<int>(delayBuffer.size());
    }
}
