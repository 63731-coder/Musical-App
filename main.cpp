#include <iostream>

#include "AudioGenerator.h"
#include "gui/MainWindow.h"

int main() {
    LockedSynthParameters sharedParams;

    AudioGenerator generator(sharedParams);
    generator.init();

    MainWindow window(sharedParams);
    window.init();
    window.run();

    return 0;
}