#include "MainWindow.h"

#include <cmath>
#include <iostream>
#include <thread>
#include <unordered_map>
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"

constexpr float FRAMERATE = 60.0f;
constexpr std::chrono::duration<double, std::milli> TARGET_FRAMETIME(1000.0 / FRAMERATE);

MainWindow::MainWindow(LockedSharedAudioParams& sharedParams)
    : params(sharedParams) {}

void MainWindow::init() {
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
        SDL_Log("Error: SDL_Init(): %s\n", SDL_GetError());
        return;
    }

    Uint32 window_flags = SDL_WINDOW_HIDDEN;
    window = SDL_CreateWindow("", 800, 750, window_flags);
    if (!window) {
        SDL_Log("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
        return;
    }

    renderer = SDL_CreateRenderer(window, nullptr);
    SDL_SetRenderVSync(renderer, 1);
    if (!renderer) {
        SDL_Log("Error: SDL_CreateRenderer(): %s\n", SDL_GetError());
        return;
    }

    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_ShowWindow(window);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavEnableGamepad;

    ImGui::StyleColorsDark();
    ImGui::GetStyle().WindowRounding = 0.0f;

    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);
}

void MainWindow::run() {
    const auto clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    std::unordered_map<SDL_Scancode, int> keyToNote = {
        {SDL_SCANCODE_Q, 0}, {SDL_SCANCODE_Z, 1}, {SDL_SCANCODE_S, 2}, {SDL_SCANCODE_E, 3},
        {SDL_SCANCODE_D, 4}, {SDL_SCANCODE_F, 5}, {SDL_SCANCODE_T, 6}, {SDL_SCANCODE_G, 7},
        {SDL_SCANCODE_Y, 8}, {SDL_SCANCODE_H, 9}, {SDL_SCANCODE_U, 10}, {SDL_SCANCODE_J, 11}
    };

    bool done = false;
    while (!done) {
        auto frameStart = std::chrono::high_resolution_clock::now();

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL3_ProcessEvent(&event);
            if (event.type == SDL_EVENT_QUIT ||
                (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED &&
                 SDL_GetWindowID(window) == event.window.windowID)) {
                done = true;
            }

            // Gérer touche pressée
            if (event.type == SDL_EVENT_KEY_DOWN) {
                auto it = keyToNote.find(event.key.scancode);
                if (it != keyToNote.end()) {
                    auto current = params.getCopy();
                    current.notePressed = true;
                    current.noteIndex = it->second;
                    current.pressedKey = event.key.scancode;
                    params = current;
                }
            }

            // Gérer touche relâchée
            if (event.type == SDL_EVENT_KEY_UP) {
                auto it = keyToNote.find(event.key.scancode);
                if (it != keyToNote.end()) {
                    auto current = params.getCopy();
                    current.notePressed = false;
                    current.pressedKey = -1;
                    params = current;
                }
            }
        }

        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        draw();

        ImGui::Render();
        SDL_SetRenderDrawColorFloat(renderer, clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        SDL_RenderClear(renderer);
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
        SDL_RenderPresent(renderer);

        auto frameEnd = std::chrono::high_resolution_clock::now();
        auto frameDuration = frameEnd - frameStart;
        if (frameDuration < TARGET_FRAMETIME) {
            std::this_thread::sleep_for(TARGET_FRAMETIME - frameDuration);
        }
    }

    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void MainWindow::draw() {
    ImGui::Begin("Synth");

    SharedAudioParamsData current = params.getCopy();

    ImGui::Checkbox("Oscillator 1", &current.oscillator1Enabled);
    ImGui::SameLine();
    ImGui::Checkbox("Oscillator 2", &current.oscillator2Enabled);

    const char* waveformItems[] = { "SINE", "SQUARE", "SAW" };
    int waveformIndex = current.oscillator1WaveForm;
    if (ImGui::Combo("OSC1 waveform", &waveformIndex, waveformItems, IM_ARRAYSIZE(waveformItems))) {
        current.oscillator1WaveForm = waveformIndex;
    }

    static float freqOffset = 0.0f;
    if (ImGui::SliderFloat("Freq Offset OSC1", &freqOffset, -5.0f, 5.0f, "%.3f")) {
        current.oscillator1Frequency = 440.0f + freqOffset;
    }

    ImGui::SliderFloat("Attack", &current.attack, 0.001f, 1.0f, "%.3f s");
    ImGui::SliderFloat("Release", &current.release, 0.001f, 2.0f, "%.3f s");
    ImGui::SliderFloat("Filter cutoff", &current.cutoff, 20.0f, 20000.0f, "%.1f Hz", ImGuiSliderFlags_Logarithmic);
    ImGui::SliderFloat("Filter resonance", &current.resonance, 0.0f, 1.0f, "%.2f");
    ImGui::SliderFloat("Delay Time", &current.delayTime, 0.1f, 1.0f, "%.2f s");
    ImGui::SliderFloat("Delay Mix", &current.delayMix, 0.0f, 1.0f, "%.2f");

    params = current;

    // 🎹 Clavier virtuel
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Text("Virtual Keyboard");
    for (int i = 0; i < 12; ++i) {
        std::string label = std::to_string(i + 1);
        if (ImGui::Button(label.c_str(), ImVec2(32, 32))) {
            auto temp = params.getCopy();
            temp.notePressed = true;
            temp.noteIndex = i;
            params = temp;

            std::thread([&, i]() {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                auto after = params.getCopy();
                after.notePressed = false;
                params = after;
            }).detach();
        }
        if (i < 11) ImGui::SameLine();
    }

    ImGui::End();
}
