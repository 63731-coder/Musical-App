#ifndef TESTINSTRUCT_MAINWINDOW_H
#define TESTINSTRUCT_MAINWINDOW_H

#include <SDL3/SDL.h>
#include "../SharedAudioParams.h"


class MainWindow {
public:
    explicit MainWindow(LockedSharedAudioParams& sharedParams);  // constructeur

    void init();
    void run();

private:
    void draw();

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    LockedSharedAudioParams& params;
};

#endif //TESTINSTRUCT_MAINWINDOW_H
