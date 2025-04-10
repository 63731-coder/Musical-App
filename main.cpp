#include <iostream>
#include "audio/Oscillator.h"
#include "AudioGenerator.h"


int main() {
    //TODO: one thread for Audio, one thread for GUI
    // if we import key codes, we need to import those from dreamGUI
    // at page 7 we have the order in which we have to apply the filters
    Oscillator osc;
    AudioGenerator audio;

    osc.setWaveform(WaveformType::SAW);
    osc.setFrequency(440.0f);

    audio.init(&osc);
    std::cin.get(); // Tap to stop
    return 0;
}
