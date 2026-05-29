#pragma once
#include <string>
#include <atomic>
#include <SDL3/SDL.h>
#include <PaperStocking/settings/settingsManager.hpp>
namespace PaperStocking {
	struct Engine {
		public:
			const static inline std::string c_Version = "1.0.0";
			static inline std::atomic_bool s_Run = true;

			Engine() = default;
			~Engine() = default;
			void Run(int argc, char* argv[]);
		private:
			static void RenderThread();
			static void GameThread();

			static void EventListener(SDL_Event& event);
	};
}
