#include <iostream>
#include "audio/Oscillator.h"
#include "AudioGenerator.h"


int main() {
    //TODO: un thread pour Audio, un thread pour GUI
    // si on importe des codes pour les touches, faut importer celles de dreamGUI
    Oscillator osc;
    AudioGenerator audio;

    osc.setWaveform(WaveformType::SQUARE);
    osc.setFrequency(440.0f);

    audio.init();
    std::cin.get(); // Tap to stop
    return 0;
}
