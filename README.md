# Synthesizer Project (4DEV4D - C++)

## Overview
This project consists of implementing a **digital subtractive synthesizer** in C++ as part of the 4DEV4D course.  
The synthesizer generates and manipulates sounds using oscillators, filters, envelopes, and effects such as delay.  
It includes both an **audio engine** and a **graphical user interface (GUI)**.

---

## Features
- **Two oscillators** (OSC1 and OSC2)
  - OSC1: selectable waveform (sine, square, sawtooth)
  - OSC2: fixed sawtooth waveform
  - Frequency offset parameter for OSC1 [-5Hz : +5Hz]
- **Envelope (ADSR simplified)**  
  - Attack [0s : 1s]  
  - Release [0s : 2s]
- **Low-pass filter**  
  - Cutoff frequency [20Hz : 20kHz]  
  - Resonance [0–1]
- **Delay effect**  
  - Time [0.1s : 2s]  
  - Mix [0–1]
- **Virtual keyboard** with 13 playable notes
- **Keyboard shortcuts** (AZERTY layout) for note triggering: `q,z,s,e,d,f,t,g,y,h,u,j,k`
- **Monophonic mode**: only one note can play at a time

---

## 🛠 Technical Details
- **Language & Standard**: C++23  
- **Cross-platform**: must work on Windows, Linux, and Mac  
- **Audio library**: [PortAudio](http://www.portaudio.com/)  
  - Stereo output (44100Hz sample rate, buffer size 256 frames)  
  - Interleaved audio buffer format  
- **GUI library**: [Dear ImGui](https://github.com/ocornut/imgui)  
- **Threading**  
  - Audio runs in a dedicated thread (callback function)  
  - GUI runs in the main thread  
  - Shared parameters must be thread-safe  
- **Architecture**  
  - Clear separation between audio engine and GUI  
  - Each module (oscillator, filter, envelope, delay) implemented as a class  
  - Audio modules process buffers sequentially:  
    `OSC → Envelope → Filter → Delay → Output`

---

## 🎛 Modules
- **Oscillators**: sine, square, sawtooth waves
- **Envelope (Attack/Release)**: modifies amplitude over time
- **Filter**: low-pass filter with cutoff and resonance
- **Delay**: echo effect with circular buffer
- **Note handling**: note-on/note-off with frequency calculation

---

## Project Setup
- Recommended IDE: **CLion** (but any C++ IDE works)
- Dependencies (provided on POESI):
  - SDL3
  - Dear ImGui
  - PortAudio

### Build Instructions
```bash
mkdir build
cd build
cmake ..
make
./synth
```

---
