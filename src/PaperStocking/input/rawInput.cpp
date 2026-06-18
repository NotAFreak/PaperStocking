#pragma once
#include <cassert>
#include <SDL3/SDL.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl3.h>
#include <PaperStocking/input/rawInput.hpp>

using namespace PaperStocking;

RawInput::RawInput() {
    assert(instance == nullptr);
    instance = this;
}
RawInput::~RawInput() {
    assert(instance != nullptr);
    instance->listeners.clear();
    instance = nullptr;
}

void RawInput::PollEvents() {
    assert(instance != nullptr);
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL3_ProcessEvent(&event);
        for (void(*listener)(SDL_Event&) : instance->listeners) {
            listener(event);
        }
    }
}

void RawInput::SubscribeListener(void (*listener)(SDL_Event&)) {
    assert(instance != nullptr);
    instance->listeners.emplace(listener);
}
void RawInput::UnsubscribeListener(void (*listener)(SDL_Event&)) {
    assert(instance != nullptr);
    instance->listeners.erase(listener);
}
