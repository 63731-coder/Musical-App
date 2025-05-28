//
// Created by Nicoleta on 18-04-25.
//

#ifndef LOWPASSFILTER_H
#define LOWPASSFILTER_H

class LowPassFilter {
public:
    explicit LowPassFilter();

    void setCutoffFrequencyHz(float newCutoffFrequencyHz);

    void setResonance(float newResonance);

    float process(float audioBuffer);

private:
    void updateCoefficients();

    float cutoffFrequencyHz = 1000.0f; ;
    float resonance = 0.5f;

    float a0 = 0.0f, a1 = 0.0f, a2 = 0.0f;
    float b1 = 0.0f, b2 = 0.0f;

    float prevInput1 = 0.0f, prevInput2 = 0.0f;
    float prevOutput1 = 0.0f, prevOutput2 = 0.0f;
};
#endif //LOWPASSFILTER_H
