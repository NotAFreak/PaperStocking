#pragma once
#include <SDL3/SDL.h>
#include <PaperStocking/components/cmp_Transform.hpp>

namespace PaperStocking {
	struct KeyboardState {
		public:
			int m_KeyCount;
			std::vector<bool> m_Keys;
			KeyboardState();
			~KeyboardState();
			// Scancodes refer to physical key positions
			bool IsScancode(const SDL_Scancode& scancode);

			// Scancodes refer to what the keys themselves are
			bool IsKeycode(const SDL_Keycode& keycode);
	};
	struct Keyboard {
		public:
			KeyboardState m_PrevState;
			KeyboardState m_CurrState;

			Keyboard();
			~Keyboard() = default;
			void Update();

			bool IsScancodeDown(const SDL_Scancode& scancode);
			bool IsScancodeUp(const SDL_Scancode& scancode);
			bool IsScancode(const SDL_Scancode& scancode);

			bool IsKeycodeDown(const SDL_Keycode& keycode);
			bool IsKeycodeUp(const SDL_Keycode& keycode);
			bool IsKeycode(const SDL_Keycode& keycode);
	};
}