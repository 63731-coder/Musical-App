
#ifndef TESTINSTRUCT_MAINWINDOW_H
#define TESTINSTRUCT_MAINWINDOW_H

#include <SDL3/SDL.h>
#include "../audio/Oscillator.h"
#include "../AudioGenerator.h"



class MainWindow {
public :
    void init();
    void run();
    Oscillator* osc1 = nullptr; // main window shouldn't know that there is an oscillator -
    Oscillator* osc2 = nullptr;

    //TODO:
    /**
    * Comme indiqué sur le diagramme de la figure 5, la boucle audio et la boucle de l’interface
    graphique tourne sur deux threads différents. La boucle audio va écrire tous les paramètres
    nécessaires à la génération du son à chaque itération. La boucle audio va lire ces paramètres à
    chaque itération. Les deux threads partageant la même ressource, les paramètres, il faut faire
    attention à ce que l’accès à ceux-ci soit thread-safe.

    Le main window doit collecter des parametres (page 8 de l'enoncé) -> soit des booleans soit de chiffres folottons
    ou qlq chose d'autre
     */

    AudioCallbackData* callbackData = nullptr;
    AudioGenerator* audio = nullptr;
private:
    void draw();
    SDL_Window* window { nullptr };
    SDL_Renderer* renderer { nullptr };
};

#endif //TESTINSTRUCT_MAINWINDOW_H
