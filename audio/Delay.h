//
// Created by Nicoleta on 24-04-25.
//

#ifndef DELAY_H
#define DELAY_H
#include <cstddef>
#include <vector>


class Delay {
public:
    Delay();

    void setDelayTime(float seconds);     // delay time [in seconds]
    void setFeedback(float fb);           // feedback amount [0.0 - 0.95]

    float process(float input);           // applies the delay effect

private:
    std::vector<float> buffer;            // circular buffer memory
    std::size_t bufferSize = 0;           // actual size of the buffer
    std::size_t writeIndex = 0;           // current write position

    float feedback = 0.5f;                // feedback level
    float delayTime = 0.3f;               // delay duration in seconds
    std::size_t delaySamples = 0;         // delay duration in samples

    void updateBufferSize();              // updates buffer size based on delay time
};


#endif //DELAY_H
