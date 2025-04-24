//
// Created by Nicoleta on 17-04-25.
//

#ifndef SIMPLE_SYNTH_ENVELOPE_H
#define SIMPLE_SYNTH_ENVELOPE_H

class Envelope {
public:
    Envelope();

    void setAttackTime(float seconds);
    void setReleaseTime(float seconds);

    void noteOn();   // attack
    void noteOff();  // release

    float nextSample();  // The current amplitude (0.0 to 1.0)

private:
    enum class State {
        IDLE, //inactive
        ATTACK,
        SUSTAIN, //between attack and release
        RELEASE
    };

    State state = State::IDLE;

    float attackTime = 0.01f;   // in seconds
    float releaseTime = 0.2f;   // in seconds

    float amplitude = 0.0f;     // current level (between 0.0 and 1.0)
    float attackIncrement = 0.0f;
    float releaseIncrement = 0.0f;

    void updateIncrements();  // recalculate the execution speed
};

#endif // SIMPLE_SYNTH_ENVELOPE_H

