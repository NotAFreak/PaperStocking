#pragma once
#include "uiDocument.hpp"
#include <RmlUi/Core.h>
namespace PaperStocking {
	struct UiManager {
		public:
			UiManager();
			~UiManager();

			void Initialize();
			void Shutdown();
			void Update();
			void Render();
		private:
			static inline UiManager* s_Instance;

			Rml::Context* m_Context;
			bool m_UiDebugVisible = false;

			static void EventListener(SDL_Event& event);
	};
}