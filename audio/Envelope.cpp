//
// Created by Nicoleta on 17-04-25.
//

#include "Envelope.h"
#include <algorithm>
#include "../utils/Constants.h"

void Envelope::setParameters(double attackTimeSeconds, double releaseTimeSeconds) {
    _attackTime = attackTimeSeconds;
    _releaseTime = releaseTimeSeconds;


    _attackFrames = static_cast<int>(Constants::SampleRate * _attackTime);
    _releaseFrames = static_cast<int>(Constants::SampleRate * _releaseTime);
}

void Envelope::noteOn() {
    if (_stage == State::ATTACK) {
        return;
    }

    _stage = State::ATTACK;
    _frameCounter = 0;


    if (_amplitude > 0.0f && _attackFrames > 0) {
        _frameCounter = static_cast<int>(_amplitude * _attackFrames);
    }
}

void Envelope::noteOff() {
    if (_stage == State::RELEASE || _stage == State::IDLE) {
        return;
    }

    _stage = State::RELEASE;
    _frameCounter = 0;


    if (_amplitude < 1.0f && _releaseFrames > 0) {
        _frameCounter = static_cast<int>((1.0f - _amplitude) * _releaseFrames);
    }
}

void Envelope::process(float *audioBuffer) {
    for (int frame = 0; frame < Constants::FramesPerBuffer; ++frame) {
        switch (_stage) {
            case State::ATTACK:
                if (_attackFrames > 0) {
                    _amplitude = static_cast<float>(_frameCounter) / _attackFrames;
                    if (_amplitude >= 1.0f) {
                        _amplitude = 1.0f;
                        _stage = State::SUSTAIN;
                    } else {
                        _frameCounter++;
                    }
                } else {

                    _amplitude = 1.0f;
                    _stage = State::SUSTAIN;
                }
                break;

            case State::SUSTAIN:
                _amplitude = 1.0f;
                break;

            case State::RELEASE:
                if (_releaseFrames > 0) {
                    _amplitude = 1.0f - (static_cast<float>(_frameCounter) / _releaseFrames);
                    if (_amplitude <= 0.0f) {
                        _amplitude = 0.0f;
                        _stage = State::IDLE;
                    } else {
                        _frameCounter++;
                    }
                } else {

                    _amplitude = 0.0f;
                    _stage = State::IDLE;
                }
                break;

            case State::IDLE:
                _amplitude = 0.0f;
                break;
        }

        audioBuffer[frame] *= _amplitude;
    }
}