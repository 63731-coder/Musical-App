#include "audioEngine/AudioGenerator.h"
#include "audioEngine/Synth.h"


int main() {
    double sampleRate = 44100.0;
    Synth synth(sampleRate);
    AudioGenerator audioGenerator;

    audioGenerator.init();

    return 0;
}
