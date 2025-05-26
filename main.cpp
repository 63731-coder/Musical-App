#include "AudioGenerator.h"
#include "gui/MainWindow.h"
#include "SharedAudioParams.h"

int main() {
    // Créer les paramètres partagés (thread-safe)
    LockedSharedAudioParams sharedParams;

    // Créer et initialiser l'audio
    AudioGenerator audio;
    audio.init(&sharedParams);

    // Créer la fenêtre GUI en lui passant les paramètres
    MainWindow window(sharedParams);
    window.init();
    window.run();

    return 0;
}