#pragma once
#include <SDL3/SDL.h>
#include <PaperStocking/math.hpp>
namespace PaperStocking {
	struct MouseState {
	public:
		// Note this is screen-space, so 0-Resolution
		Vector2 m_Position = {0.0f, 0.0f};
		SDL_MouseButtonFlags m_Buttons;

		MouseState();
		~MouseState() = default;

		bool IsButton(const SDL_MouseButtonFlags& button) const;
	};
	struct Mouse {
		public:
			MouseState m_PrevState;
			MouseState m_CurrState;

			Mouse();
			~Mouse() = default;

			void Update();
			bool IsButtonDown(const SDL_MouseButtonFlags& button) const;
			bool IsButtonUp(const SDL_MouseButtonFlags& button) const;
			bool IsButton(const SDL_MouseButtonFlags& button) const;
	};
}