#pragma once
#include "SDL3/SDL_events.h"
#include <set>
namespace PaperStocking {
    struct RawInput {
        public:
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

