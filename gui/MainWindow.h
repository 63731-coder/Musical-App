
#ifndef TESTINSTRUCT_MAINWINDOW_H
#define TESTINSTRUCT_MAINWINDOW_H

#include <SDL3/SDL.h>
#include "../audio/Oscillator.h"

class MainWindow {
public :
    void init();
    void run();
    Oscillator* osc = nullptr;
private:
    void draw();
    SDL_Window* window { nullptr };
    SDL_Renderer* renderer { nullptr };
};

#endif //TESTINSTRUCT_MAINWINDOW_H
