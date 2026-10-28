#pragma once
#include <memory>
#include <filesystem>
#include <SDL3/SDL.h>
#include <PaperStocking/graphics/texture.hpp>

namespace PaperStocking {
	struct CursorVisual {
	public:
		CursorVisual(std::filesystem::path filePath, int hotspotX = 0, int hotspotY = 0);
		~CursorVisual();
	private:
		SDL_Surface* m_Surface = nullptr;

		uint16_t m_HotspotX = 64;
		uint16_t m_HotspotY = 64;
		SDL_Cursor* m_Cursor = nullptr;
	};
	enum CursorState {
		Shown,
		Hidden,
		Locked
	};
	struct Cursor {
		public:
			Cursor();
			~Cursor();

			static CursorState GetState();
			static void SetState(CursorState state);
			static std::weak_ptr<CursorVisual> GetVisual();
			static void SetVisual(std::shared_ptr<CursorVisual> visual);
		private:
			static inline Cursor* s_Instance = nullptr;
			CursorState m_State = CursorState::Shown;
	};
}
