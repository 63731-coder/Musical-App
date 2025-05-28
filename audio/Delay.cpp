//
// Created by Nicoleta on 24-04-25.
//

#include "Delay.h"
#include "../utils/Constants.h"


Delay::Delay()
    : writeIndex(0),
      delayTime(0.3f),
      mix(0.2f)
{
    delayBuffer.resize(Constants::SampleRate * 2, 0.0f);
}

void Delay::setDelayTime(float seconds) {
    delayTime = seconds;
}

void Delay::setMix(float mixValue) {
    mix = mixValue;
}

void Delay::process(float* buffer) {
    // Calcul du délai en échantillons
    int delaySamples = static_cast<int>(delayTime * Constants::SampleRate);

    for (int i = 0; i < Constants::FramesPerBuffer ; ++i) { // *2 pour stéréo
        // Calculer l'index de lecture
        int readIndex = writeIndex - delaySamples;
        if (readIndex < 0) {
            readIndex += delayBuffer.size();
        }

        // Lecture de l'échantillon retardé
        float delayedSample = delayBuffer[readIndex];

        // Application de l'algorithme de delay (selon l'énoncé)
        buffer[i] = buffer[i] + mix * delayedSample;

        // Stockage de l'échantillon actuel
        delayBuffer[writeIndex] = buffer[i];

        // Avancer l'index d'écriture
        writeIndex = (writeIndex + 1) % delayBuffer.size();
    }
}