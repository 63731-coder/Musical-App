//
// Created by Nicoleta on 24-04-25.
//
#ifndef DELAY_H
#define DELAY_H

#include <vector>

// Delay class: adds a simple audio delay (echo) effect using a circular buffer
class Delay {
public:
    Delay();

    // Set the delay time in seconds
    void setDelayTime(float seconds);

    // Set how much of the delayed signal is mixed in
    void setMix(float mixValue);

    // Apply the delay effect to a buffer of audio samples
    void process(float *buffer);

private:
    std::vector<float> delayBuffer; // Circular buffer for delayed samples
    int writeIndex; // Current write position in the delay buffer
    float delayTime; // Delay time in seconds
    float mixLevel; // Mix amount of delayed signal (0.0 - 1.0)
};

#endif // DELAY_H
