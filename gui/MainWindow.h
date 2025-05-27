#ifndef TESTINSTRUCT_MAINWINDOW_H
#define TESTINSTRUCT_MAINWINDOW_H

#include <SDL3/SDL.h>
#include "../AudioParam.h"

class MainWindow {
public:
    MainWindow(LockedSynthParameters& params);
    void init();
    void run();
private:
    void draw();

    SDL_Window* window { nullptr };
    SDL_Renderer* renderer { nullptr };

    LockedSynthParameters& params;
};
#endif //TESTINSTRUCT_MAINWINDOW_H
