#include "audio/Oscillator.h"
#include "AudioGenerator.h"
#include "gui/MainWindow.h"


int main() {
    //TODO: one thread for Audio, one thread for GUI
    // if we import key codes, we need to import those from dreamGUI
    // at page 7 we have the order in which we have to apply the filters
    Oscillator osc1;
    Oscillator osc2;
    AudioGenerator audio;

    osc1.setWaveform(WaveformType::SQUARE);
    osc1.setFrequency(440.0f);

    osc2.setWaveform(WaveformType::SAW);

    audio.init(&osc1, &osc2);

    MainWindow window;
    window.osc1 = &osc1;
    window.osc2 = &osc2;
    window.callbackData = audio.getCallbackData();
    window.audio = &audio;
    window.init();
    window.run();

    return 0;
}