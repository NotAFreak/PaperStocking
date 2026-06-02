#pragma once
#include <set>
#include "mouse.hpp"
#include "keyboard.hpp"
#include <SDL3/SDL_events.h>
namespace PaperStocking {
    struct RawInput {
        public:
            inline static Keyboard s_Keyboard;
            inline static Mouse s_Mouse;

            RawInput();
            ~RawInput();

            void PollEvents();
            static void SubscribeListener(void(*listener)(SDL_Event&));
            static void UnsubscribeListener(void(*listener)(SDL_Event&));
        private:
            static inline RawInput* instance;
            std::set<void(*)(SDL_Event&)> listeners;
    };
}

