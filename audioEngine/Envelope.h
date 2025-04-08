//
// Created by Nicoleta on 03-04-25.
//

#ifndef ENVELOPE_H
#define ENVELOPE_H
#include <vector>


class Envelope {
public:
    Envelope(double sampleRate);

    enum class Phase {
        Idle,
        Attack,
        Decay,
        Sustain,
        Release
    };

    void setAttack(double time);
    void setDecay(double time);
    void setSustain(double level);
    void setRelease(double time);
    void noteOn();
    void noteOff();
    void process(std::vector<float>& buffer);

private:
    double sampleRate;
    double currentTime;
    Phase currentPhase;
    double attackTime;
    double decayTime;
    double sustainLevel;
    double releaseTime;
    double currentLevel;
};



#endif //ENVELOPE_H
