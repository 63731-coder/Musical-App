//
// Created by Nicoleta on 17-04-25.
//

#include "Envelope.h"
#include "../utils/Constants.h"

void Envelope::setParameters(const float attackTimeSeconds, const float releaseTimeSeconds) {
    attackTime = attackTimeSeconds;
    releaseTime = releaseTimeSeconds;

    //convert them to sample frame counts
    attackFrames = static_cast<int>(Constants::SampleRate * attackTime);
    releaseFrames = static_cast<int>(Constants::SampleRate * releaseTime);
}

void Envelope::noteOn() {
    if (stage == State::ATTACK) {
        return; // already attacking
    }
    stage = State::ATTACK;
    frameCounter = 0;

    // If already partially active, resume attack from the current amplitude to avoid the 'pop' noise
    if (amplitude > 0.0f && attackFrames > 0) {
        frameCounter = static_cast<int>(amplitude * static_cast<float>(attackFrames));
    }
}

void Envelope::noteOff() {
    if (stage == State::RELEASE || stage == State::DECAY) {
        return; // already releasing or inactive
    }
    stage = State::RELEASE;
    frameCounter = 0;

    // If still above zero, release from current amplitude to avoid the 'pop' noise
    if (amplitude < 1.0f && releaseFrames > 0) {
        frameCounter = static_cast<int>((1.0f - amplitude) * static_cast<float>(releaseFrames));
    }
}

void Envelope::process(float *audioBuffer) {
    for (int frame = 0; frame < Constants::FramesPerBuffer; ++frame) {
        switch (stage) {
            case State::ATTACK:
                if (attackFrames > 0) {
                    // Increase amplitude gradually during attack
                    amplitude = static_cast<float>(frameCounter) / static_cast<float>(attackFrames);
                    if (amplitude >= 1.0f) {
                        amplitude = 1.0f;
                        stage = State::SUSTAIN; // Move to sustain once attack completes (amplitude = 1)
                    } else {
                        frameCounter++;
                    }
                } else {
                    // If attack = 0, go straight to full amplitude
                    amplitude = 1.0f;
                    stage = State::SUSTAIN;
                }
                break;

            case State::SUSTAIN:
                amplitude = 1.0f; // hold full volume
                break;

            case State::RELEASE:
                if (releaseFrames > 0) {
                    // Decrease amplitude gradually during release
                    amplitude = 1.0f - (static_cast<float>(frameCounter) / static_cast<float>(releaseFrames));
                    if (amplitude <= 0.0f) {
                        amplitude = 0.0f;
                        stage = State::DECAY; // Finished releasing
                    } else {
                        frameCounter++;
                    }
                } else {
                    // If release time is zero, go straight to zero
                    amplitude = 0.0f;
                    stage = State::DECAY;
                }
                break;

            case State::DECAY: // Stay silent
                amplitude = 0.0f;
                break;
        }

        // Apply current amplitude to the audio sample
        audioBuffer[frame] = audioBuffer[frame] * amplitude;
    }
}