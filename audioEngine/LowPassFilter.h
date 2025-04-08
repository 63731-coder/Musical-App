//
// Created by Nicoleta on 03-04-25.
//

#ifndef LOWPASSFILTER_H
#define LOWPASSFILTER_H
#include <vector>

class LowPassFilter {
public:
    LowPassFilter(double sampleRate);
    void setCutoff(double freq);
    void setResonance(double res);
    void process(std::vector<float>& buffer);

private:
    void updateFilter();

    double sampleRate;
    double cutoff;
    double resonance;
    double q, omega, alpha, cosw, norm;
    double a0, a1, a2, b1, b2;
    double x1, x2, y1, y2;
};




#endif //LOWPASSFILTER_H
