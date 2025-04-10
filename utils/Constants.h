//
// Created by Nicoleta on 10-04-25.
//

#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <numbers>

constexpr float TWO_PI = std::numbers::pi_v<float>*2;
constexpr int SAMPLE_RATE = 44100;
constexpr int FRAMES_PER_BUFFER = 256;

#endif // CONSTANTS_H
