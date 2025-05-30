#ifndef TESTINSTRUCT_MAINWINDOW_H
#define TESTINSTRUCT_MAINWINDOW_H
#include "../engine/AudioParam.h"
#include <SDL3/SDL.h>

class MainWindow {
public:
    explicit MainWindow(LockedSynthParameters &params);

    // Initializes the SDL window and renderer.
    void init();

    // Main loop of the window: handles events
    void run() const;

private:
    // Handles drawing visual elements to the screen.
    void draw() const;

    SDL_Window *window{nullptr}; // SDL window handle
    SDL_Renderer *renderer{nullptr}; // SDL renderer handle

    LockedSynthParameters &params; //thread-safe parameters
};

#endif //TESTINSTRUCT_MAINWINDOW_H
