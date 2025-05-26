//
// Created by Nicoleta on 10-04-25.
//

#ifndef OSCILLATOR_H
#define OSCILLATOR_H



class Oscillator {
public:
    // Formes d'onde disponibles
    enum class Waveform {
        SINE,
        SQUARE,
        SAW
    };

    explicit Oscillator();

    // Définit la fréquence de l'oscillateur en Hz
    void setFrequency(double newFrequencyHz);

    // Définit la forme d'onde à générer
    void setWaveform(Waveform newWaveform);

    // Définit la fréquence d’échantillonnage
    void setSampleRate(double newSampleRate);

    // Réinitialise la phase de l’oscillateur
    void resetPhase();

    // Remplit un buffer audio stéréo avec des samples générés
    void process(float* audioBuffer);

private:
    double currentSampleRate;
    double currentFrequencyHz;
    double phaseRadians;
    Waveform waveformType;
};



#endif //OSCILLATOR_H
