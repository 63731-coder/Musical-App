#include "MainWindow.h"
#include <thread>
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"
#include <cmath>
#include "../utils/Constants.h"

constexpr float FRAMERATE = 60.0f;
constexpr std::chrono::duration<double, std::milli> TARGET_FRAMETIME(1000.0 / FRAMERATE);

MainWindow::MainWindow(LockedSynthParameters &params)
    : params(params) {
}

void MainWindow::init() {
    // Setup SDL
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
        SDL_Log("Error: SDL_Init(): %s", SDL_GetError());
        return;
    }
    // Create window with SDL_Renderer graphics context
    Uint32 window_flags = SDL_WINDOW_HIDDEN;
    window = SDL_CreateWindow("", 750, 500, window_flags);
    if (nullptr == window) {
        SDL_Log("Error: SDL_CreateWindow(): %s", SDL_GetError());
        return;
    }
    renderer = SDL_CreateRenderer(window, nullptr);
    SDL_SetRenderVSync(renderer, 1);
    if (nullptr == renderer) {
        SDL_Log("Error: SDL_CreateRenderer(): %s", SDL_GetError());
        return;
    }
    SDL_SetWindowPosition(
        window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_ShowWindow(window);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    // Setup DearImGui style
    ImGui::StyleColorsLight();
    ImGui::GetStyle().WindowRounding = 0.0f;

    // Setup Platform/Renderer backends
    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);
}

void MainWindow::run() {
    const auto clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    bool done{false};
    while (!done) {
        auto frameStart = std::chrono::high_resolution_clock::now();

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL3_ProcessEvent(&event);
            if (SDL_EVENT_QUIT == event.type)
                done = true;
            if ((SDL_EVENT_WINDOW_CLOSE_REQUESTED == event.type)
                && (SDL_GetWindowID(window) == event.window.windowID))
                done = true;
        }

        // Start the Dear ImGui frame
        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        // all the UI code description
        draw();

        // Rendering
        ImGui::Render();
        SDL_SetRenderDrawColorFloat(renderer,
                                    clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        SDL_RenderClear(renderer);
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
        SDL_RenderPresent(renderer);

        // Calculate time spent and sleep if needed
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
    ImGui::SetNextWindowSize(ImVec2(660, 400));
    ImGui::Begin("Synthétiseur", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

    SynthParameters currentState = params.getCopy();

    // --- OSC1 et OSC2 (checkboxes) ---
    ImGui::Checkbox("OSC 1", &currentState.osc1Active);

    // --- OSC1 Waveform (menu déroulant) ---
    const char *waveforms[] = {"SINE", "SQUARE", "SAW"};
    ImGui::SetNextItemWidth(Constants::widthControls);
    ImGui::Combo("OSC1 Waveform", &currentState.osc1Waveform, waveforms, IM_ARRAYSIZE(waveforms));

    // --- OSC1 Frequency Offset ---
    ImGui::SetNextItemWidth(Constants::widthControls);
    ImGui::SliderFloat("OSC1 Frequency Offset", &currentState.osc1FrequencyOffsetHz, -5.0f, 5.0f, "%.1f Hz");

    // --- OSC2 (checkbox) ---
    ImGui::Checkbox("OSC 2", &currentState.osc2Active);

    // --- Attack slider ---
    ImGui::SetNextItemWidth(Constants::widthControls);
    ImGui::SliderFloat("Attack", &currentState.envelopeAttackSec, 0.0f, 1.0f, "%.2f sec");

    // --- Release slider ---
    ImGui::SetNextItemWidth(Constants::widthControls);
    ImGui::SliderFloat("Release", &currentState.envelopeReleaseSec, 0.0f, 2.0f, "%.2f sec");

    // --- Filter Cutoff slider ---
    ImGui::SetNextItemWidth(Constants::widthControls);
    ImGui::SliderFloat("Filter Cutoff", &currentState.filterCutoffHz, Constants::MinCutoff, Constants::MaxCutoff,
                       "%.0f Hz", ImGuiSliderFlags_Logarithmic);

    // --- Filter Resonance slider ---
    ImGui::SetNextItemWidth(Constants::widthControls);
    ImGui::SliderFloat("Filter Resonance", &currentState.filterResonance, 0.01f, 0.99f, "%.2f");

    // --- Delay Time slider ---
    ImGui::SetNextItemWidth(Constants::widthControls);
    ImGui::SliderFloat("Delay Time", &currentState.delayTimeSec, 0.1f, 2.0f, "%.2f sec");

    // --- Delay Mix slider ---
    ImGui::SetNextItemWidth(Constants::widthControls);
    ImGui::SliderFloat("Delay Mix", &currentState.delayMix, 0.0f, 1.0f, "%.2f");


    // --- Clavier virtuel (13 boutons) ---
    ImGui::Separator();

    const char *noteNames[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13"};
    const ImGuiKey keyMap[] = {
        ImGuiKey_Q, ImGuiKey_Z, ImGuiKey_S, ImGuiKey_E, ImGuiKey_D,
        ImGuiKey_F, ImGuiKey_T, ImGuiKey_G, ImGuiKey_Y, ImGuiKey_H,
        ImGuiKey_U, ImGuiKey_J, ImGuiKey_K
    };

    bool isAnyKeyPressed = false;

    // Vérifier d'abord les touches du clavier
    for (int i = 0; i < 13; ++i) {
        if (ImGui::IsKeyDown(keyMap[i])) {
            isAnyKeyPressed = true;
            currentState.activeNote = true;
            currentState.noteIndex = i;
            break; // On ne joue qu'une note à la fois
        }
    }

    // Afficher les boutons et détecter les clics de souris
    for (int i = 0; i < 13; ++i) {
        ImGui::PushID(i);

        // Mettre le bouton en surbrillance si c'est la note active
        bool isActive = (currentState.noteIndex == i && currentState.activeNote);
        if (isActive) {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.7f, 0.9f, 1.0f));
        }

        ImGui::Button(noteNames[i], ImVec2(30, 30));

        if (ImGui::IsItemActive()) {
            isAnyKeyPressed = true;
            currentState.activeNote = true;
            currentState.noteIndex = i;
        }

        if (isActive) {
            ImGui::PopStyleColor();
        }

        if (i < 12) ImGui::SameLine();

        ImGui::PopID();
    }

    if (!isAnyKeyPressed) {
        currentState.activeNote = false;
    }
    params.upDate(currentState);

    ImGui::End();
}
