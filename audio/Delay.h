//
// Created by Nicoleta on 24-04-25.
//

#ifndef DELAY_H
#define DELAY_H
#include <vector>


class Delay {
public:
    // Constructeur par défaut
    Delay();

    // Méthodes pour définir les paramètres
    void setDelayTime(float seconds);
    void setMix(float mixValue);

    // Méthode pour traiter le son
    void process(float* buffer);

private:
    std::vector<float> delayBuffer;
    int writeIndex;
    float delayTime;
    float mix;
};


#endif //DELAY_H
