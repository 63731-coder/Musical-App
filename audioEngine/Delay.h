//
// Created by Nicoleta on 03-04-25.
//

#ifndef DELAY_H
#define DELAY_H
#include <vector>

class Delay {
public:
    Delay(double sampleRate);
    void setDelayTime(double time);
    void setDelayMix(double mix);
    void process(std::vector<float>& buffer);

private:
    double sampleRate;
    double delayTime;
    double delayMix;
    std::vector<float> buffer;
    size_t readIndex, writeIndex;
};

#endif //DELAY_H
