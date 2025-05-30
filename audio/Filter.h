//
// Created by Nicoleta on 19-04-25.
//

#ifndef FILTER_H
#define FILTER_H

class Filter {
public:
    Filter();

    // Set the filter's cutoff frequency in Hertz
    void setCutoffFrequencyHz(float newCutoffFrequencyHz);

    // Set the filter's resonance (0 = no resonance, closer to 1 = more peak at cutoff)
    void setResonance(float newResonance);

    // Apply the filter to one audio sample
    float process(float input);

private:
    // Recalculate filter coefficients based on cutoff and resonance
    void updateCoefficients();

    // Filter settings
    float cutoffFrequencyHz; // Where the filter starts cutting
    float resonance; // How strong the peak is at the cutoff point

    // Coefficients used in the filter formula
    float a0 = 0.0f, a1 = 0.0f, a2 = 0.0f;
    float b1 = 0.0f, b2 = 0.0f;

    // Stores past inputs and outputs for filtering
    float prevInput1 = 0.0f, prevInput2 = 0.0f;
    float prevOutput1 = 0.0f, prevOutput2 = 0.0f;
};

#endif //FILTER_H
