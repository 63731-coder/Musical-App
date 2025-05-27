//
// Created by Nicoleta on 17-04-25.
//

#ifndef SIMPLE_SYNTH_ENVELOPE_H
#define SIMPLE_SYNTH_ENVELOPE_H

class Envelope {
public:
    // États de l'enveloppe
    enum class State {
        IDLE,
        ATTACK,
        SUSTAIN,
        RELEASE,
    };

    explicit Envelope(double initialSampleRate = 44100.0);

    void setSampleRate(double newSampleRate);
    void setParameters(double attackTimeSeconds, double releaseTimeSeconds);

    void noteOn();   // Déclenchement de la note (attaque)
    void noteOff();  // Fin de la note (relâchement)

    void process(float* audioBuffer);  // Applique l'enveloppe au buffer audio
    bool isRunning() const;            // Indique si une note est active

private:
    void enterState(State newState);   // Change d'état et initialise les compteurs

    State currentState;

    double sampleRate;
    double envelopeValue;

    double attackDurationSeconds;      // Durée de l'attaque
    double releaseDurationSeconds;     // Durée du relâchement

    int elapsedSamplesInStage;         // Compteur dans l'étape en cours
    int totalSamplesInStage;           // Nombre total de samples dans l'étape
};

#endif // SIMPLE_SYNTH_ENVELOPE_H

