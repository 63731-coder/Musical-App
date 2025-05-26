//
// Created by Nicoleta on 15-05-25.
//

#ifndef SHAREDAUDIOPARAMS_H
#define SHAREDAUDIOPARAMS_H

#pragma once
#include <mutex>


// Forward declaration
struct LockedSharedAudioParams;

// POD contenant tous les paramètres audio partagés
struct SharedAudioParamsData {
    // 🔊 Oscillateurs
    bool oscillator1Enabled = true;
    int oscillator1WaveForm = 0; // SINE = 0, SQUARE = 1, SAW = 2
    float oscillator1Frequency = 440.0f;
    double oscillator1Phase = 0.0;

    bool oscillator2Enabled = false;
    int oscillator2WaveForm = 2;
    float oscillator2Frequency = 440.0f;
    double oscillator2Phase = 0.0;

    // Enveloppe (ADSR simplifiée)
    float attack = 0.05f;   // seconds
    float release = 0.75f;  // seconds

    // Filtre passe-bas
    float cutoff = 1000.0f;      // Hz
    float resonance = 0.5f;      // [0.0 - 1.0]

    // Effet de délai
    float delayTime = 0.3f;      // seconds
    float delayMix = 0.5f;       // [0.0 - 1.0]

    // Interface
    int pressedKey = -1;
    bool notePressed = false;
    int noteIndex = 0; // de 0 à 12

    // Copie thread-safe depuis un LockedSharedAudioParams
    SharedAudioParamsData& operator=(const LockedSharedAudioParams& other);
};

// Version thread-safe de la structure
struct LockedSharedAudioParams {
    mutable std::mutex mtx;
    SharedAudioParamsData data;

    // Affectation thread-safe depuis une version non verrouillée
    void operator=(const SharedAudioParamsData& other) {
        std::lock_guard<std::mutex> lock(mtx);
        data = other;
    }

    // Lecture thread-safe (copie des données)
    SharedAudioParamsData getCopy() const {
        std::lock_guard<std::mutex> lock(mtx);
        return data;
    }
};

// Implémentation de l'opérateur = de POD depuis un LockedPOD
inline SharedAudioParamsData& SharedAudioParamsData::operator=(const LockedSharedAudioParams& other) {
    std::lock_guard<std::mutex> lock(other.mtx);
    *this = other.data;
    return *this;
}


#endif //SHAREDAUDIOPARAMS_H
