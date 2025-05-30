//
// Created by Nicoleta on 10-04-25.
//

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <numbers>

namespace Constants {
    // Math constants
    constexpr auto TWO_PI = std::numbers::pi_v<float> * 2.0f;

    // Audio settings
    constexpr int SampleRate = 44100; // Samples per second
    constexpr int FramesPerBuffer = 256; // Audio buffer size

    // UI layout settings
    constexpr int widthControls = 400; // Width of the control panel in pixels

    // Filter frequency limits
    constexpr float MinCutoff = 20.0f; // Minimum cutoff frequency (Hz)
    constexpr float MaxCutoff = 20000.0f; // Maximum cutoff frequency (Hz)
}

#endif // CONSTANTS_H
