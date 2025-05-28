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
    delayBuffer.resize(Constants::SampleRate * 2, 0.0f);
}

void Delay::setDelayTime(float seconds) {
    delayTime = seconds;
}

void Delay::setMix(float mixValue) {
    mix = mixValue;
}

void Delay::process(float *buffer) {
    int delaySamples = static_cast<int>(delayTime * Constants::SampleRate);

    for (int i = 0; i < Constants::FramesPerBuffer; ++i) {
        int readIndex = writeIndex - delaySamples;
        if (readIndex < 0) {
            readIndex += delayBuffer.size();
        }

        float delayedSample = delayBuffer[readIndex];

        buffer[i] = buffer[i] + mix * delayedSample;

        delayBuffer[writeIndex] = buffer[i];

        writeIndex = (writeIndex + 1) % delayBuffer.size();
    }
}
