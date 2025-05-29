//
// Created by Nicoleta on 17-04-25.
//

#ifndef ENVELOPE_H
#define ENVELOPE_H

// Envelope class: applies a simplified amplitude envelope (attack and release) to audio samples
class Envelope {
public:
    enum class State {
        DECAY, // amplitude = 0
        ATTACK, // increasing amplitude
        SUSTAIN, // maximum amplitude
        RELEASE, // decreasing amplitude
    };

    // Set the attack and release times (in seconds)
    void setParameters(float attackTimeSeconds, float releaseTimeSeconds);

    // Trigger the envelope (enter ATTACK phase)
    void noteOn();

    // Release the envelope (enter RELEASE phase)
    void noteOff();

    // Apply the envelope to the given audio buffer, frame by frame
    void process(float *audioBuffer);

private:
    float attackTime{0.5f};
    float releaseTime{1.0f};
    float amplitude{0.0f};
    State stage{State::DECAY};

    int frameCounter = 0; // How many frames into ATTACK or RELEASE we are
    int attackFrames = 0; // Total number of frames needed for attack
    int releaseFrames = 0; // Total number of frames needed for release
};

#endif // ENVELOPE_H
