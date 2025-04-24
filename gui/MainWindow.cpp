
#include "MainWindow.h"

#include <cmath>
#include <iostream>
#include <thread>
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"
#include <unordered_map>


constexpr float FRAMERATE = 60.0f;
constexpr std::chrono::duration<double, std::milli> TARGET_FRAMETIME(1000.0 / FRAMERATE);

void MainWindow::init() {

    // Setup SDL
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
        SDL_Log("Error: SDL_Init(): %s\n", SDL_GetError());
        return;
    }
    // Create window with SDL_Renderer graphics context
    Uint32 window_flags = SDL_WINDOW_HIDDEN;
    window = SDL_CreateWindow("", 800, 750, window_flags);
    if (nullptr == window) {
        SDL_Log("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
        return;
    }
    renderer = SDL_CreateRenderer(window, nullptr);
    SDL_SetRenderVSync(renderer, 1);
    if (nullptr == renderer) {
        SDL_Log("Error: SDL_CreateRenderer(): %s\n", SDL_GetError());
        return;
    }
    SDL_SetWindowPosition(
            window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_ShowWindow(window);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    // Setup DearImGui style
    ImGui::StyleColorsDark();
    ImGui::GetStyle().WindowRounding = 0.0f;

    // Setup Platform/Renderer backends
    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);
}

void MainWindow::run() {
    const auto clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Table de correspondance : touche physique -> note MIDI index (0 à 11)
    std::unordered_map<SDL_Scancode, int> keyToNote = {
        {SDL_SCANCODE_Q, 0},
        {SDL_SCANCODE_Z, 1},
        {SDL_SCANCODE_S, 2},
        {SDL_SCANCODE_E, 3},
        {SDL_SCANCODE_D, 4},
        {SDL_SCANCODE_F, 5},
        {SDL_SCANCODE_T, 6},
        {SDL_SCANCODE_G, 7},
        {SDL_SCANCODE_Y, 8},
        {SDL_SCANCODE_H, 9},
        {SDL_SCANCODE_U, 10},
        {SDL_SCANCODE_J, 11}
    };

    bool done { false };
    while (!done) {
        auto frameStart = std::chrono::high_resolution_clock::now();

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL3_ProcessEvent(&event);

            if (event.type == SDL_EVENT_QUIT)
                done = true;

            if ((event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED)
                && (SDL_GetWindowID(window) == event.window.windowID))
                done = true;

            // 🎹 Gérer appui de touche
            if (event.type == SDL_EVENT_KEY_DOWN) {
                SDL_Scancode code = event.key.scancode;
                auto it = keyToNote.find(code);
                if (it != keyToNote.end()) {
                    int noteIndex = it->second;
                    float freq = 220.0f * std::pow(2.0f, noteIndex / 12.0f);
                    if (osc1 && audio) {
                        osc1->setFrequency(freq);
                        audio->noteOn();
                    }
                }
            }

            // 🎹 Gérer relâchement
            if (event.type == SDL_EVENT_KEY_UP) {
                SDL_Scancode code = event.key.scancode;
                if (keyToNote.find(code) != keyToNote.end()) {
                    if (audio) {
                        audio->noteOff();
                    }
                }
            }
        }

        // Start Dear ImGui frame
        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        draw(); // Dessiner l'interface

        // Rendering
        ImGui::Render();
        SDL_SetRenderDrawColorFloat(renderer, clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        SDL_RenderClear(renderer);
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
        SDL_RenderPresent(renderer);

        // Frame rate limiter
        auto frameEnd = std::chrono::high_resolution_clock::now();
        auto frameDuration = frameEnd - frameStart;
        if (frameDuration < TARGET_FRAMETIME) {
            std::this_thread::sleep_for(TARGET_FRAMETIME - frameDuration);
        }
    }

    // Cleanup
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void MainWindow::draw() {
    ImGui::Begin("Synth");

    if (callbackData) {
        ImGui::Checkbox("Oscillator 1", &callbackData->osc1Active);
        ImGui::SameLine();
        ImGui::Checkbox("Oscillator 2", &callbackData->osc2Active);
    }

    ImGui::Spacing();
    // Waveform selection
    static int waveformIndex = 0;
    const char* waveformItems[] = { "SINE", "SQUARE", "SAW" };
    if (ImGui::Combo("OSC1 waveform", &waveformIndex, waveformItems, IM_ARRAYSIZE(waveformItems))) {
        if (osc1) {
            switch (waveformIndex) {
                case 0: osc1->setWaveform(WaveformType::SINE); break;
                case 1: osc1->setWaveform(WaveformType::SQUARE); break;
                case 2: osc1->setWaveform(WaveformType::SAW); break;
            }
        }
    }

    // Frequency offset slider
    ImGui::Spacing();
    static float freqOffset = 0.0f;
    if (ImGui::SliderFloat("Freq Offset OSC1", &freqOffset, -5.0f, 5.0f, "%.3f")) {
        if (osc1) {
            osc1->setFrequency(440.0f + freqOffset); // base = 440Hz
        }
    }

    // Envelope
    static float attackTime = 0.5f;
    static float releaseTime = 1.0f;
    if (ImGui::SliderFloat("Attack", &attackTime, 0.001f, 1.0f, "%.3f s") ||
        ImGui::SliderFloat("Release", &releaseTime, 0.001f, 1.0f, "%.3f s")) {
        if (audio) {
            audio->setEnvelopeParams(attackTime, releaseTime);
        }
        }

    // Filter
    bool changed = false;
    static float cutoff = 20000.0f;     // Initial value
    static float resonance = 0.0f;

    changed |= ImGui::SliderFloat("Filter cutoff", &cutoff, 20.0f, 20000.0f, "%.1f Hz", ImGuiSliderFlags_Logarithmic);
    changed |= ImGui::SliderFloat("Filter resonance", &resonance, 0.0f, 1.0f, "%.2f");

    if (changed && audio) {
        audio->setFilterParams(cutoff, resonance);
    }


    // keyboard
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Text("Virtual Keyboard");
    for (int i = 0; i < 12; ++i) {
        std::string label = std::to_string(i + 1);
        if (ImGui::Button(label.c_str(), ImVec2(32, 32))) {
            if (osc1 && audio) {
                float noteFreq = 220.0f * std::pow(2.0f, i / 12.0f);
                osc1->setFrequency(noteFreq);
                audio->noteOn();

                std::thread([a = audio]() {
                    std::this_thread::sleep_for(std::chrono::milliseconds(50));
                    a->noteOff();
                }).detach();
            }
        }
        if (i < 11) ImGui::SameLine();
    }
    ImGui::End();
}
