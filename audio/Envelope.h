//
// Created by Nicoleta on 17-04-25.
//
#ifndef ENVELOPE_H
#define ENVELOPE_H

class Envelope {
public:
    enum class State {
        IDLE,
        ATTACK,
        SUSTAIN,
        RELEASE,
    };

    void setParameters(double attackTimeSeconds, double releaseTimeSeconds);

    void noteOn();
    void noteOff();


    void process(float *audioBuffer);

private:
    float _attackTime {0.5f};
    float _releaseTime {1.0f};
    float _amplitude {0.0f};
    State _stage {State::IDLE};

    int _frameCounter = 0;
    int _attackFrames = 0;
    int _releaseFrames = 0;


};

#endif // ENVELOPE_H
