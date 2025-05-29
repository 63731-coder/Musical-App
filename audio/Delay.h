//
// Created by Nicoleta on 24-04-25.
//
#ifndef DELAY_H
#define DELAY_H

#include <vector>

class Delay {
public:
    explicit Delay();

    void setDelayTime(float seconds);

    void setMix(float mixValue);

    void process(float *buffer);

private:
    std::vector<float> delayBuffer;
    int writeIndex;
    float delayTime;
    float mix;
};

#endif // DELAY_H
