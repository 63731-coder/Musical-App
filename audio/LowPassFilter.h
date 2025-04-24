//
// Created by Nicoleta on 18-04-25.
//

#ifndef LOWPASSFILTER_H
#define LOWPASSFILTER_H



class LowPassFilter {
public:
    LowPassFilter();

    void setCutoff(float cutoffHz);     // [20 Hz - 20000 Hz]
    void setResonance(float resonance); // [0.0 - 1.0]

    float process(float input);         // Apply filter to a sample

private:
    void updateCoefficients();

    float cutoff = 1000.0f;     // Default cutoff frequency
    float resonance = 0.5f;     // Default resonance

    // Filter coefficients
    float a0 = 0.0f, a1 = 0.0f, a2 = 0.0f;
    float b1 = 0.0f, b2 = 0.0f;

    // State for mono (you’ll duplicate this for stereo if needed)
    float x1 = 0.0f, x2 = 0.0f;
    float y1 = 0.0f, y2 = 0.0f;
};



#endif //LOWPASSFILTER_H
