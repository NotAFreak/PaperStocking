#pragma once
#include <vector>
#include <memory>
#include <SDL3/SDL.h>
#include <RmlUi/Core.h>
#include "uiDocument.hpp"

namespace PaperStocking {
	struct UiManager {
		public:
			UiManager();
			~UiManager();

			void Initialize();
			void Shutdown();
			void Update();
			void Render();

			// Note, DO NOT INCLUDE EXTENSION OR PATH TO IT
			static std::weak_ptr<UiDocument> LoadDocument(std::string name);
			static void UnloadDocument(std::string name);
			static void UnloadDocument(std::weak_ptr<UiDocument> doc);

			static void ReloadAllDocuments();
			static void ReloadDocument(std::string name);
			static void ReloadDocument(std::weak_ptr<UiDocument> doc);

			static void QueueEvent(UiExecThread thread, UiEventCall call);
			static void ExecuteAllEvents(UiExecThread thread);
		private:
			static inline UiManager* s_Instance;

			Rml::Context* m_Context;
			bool m_UiDebugVisible = false;
			std::map<std::string, std::shared_ptr<UiDocument>> m_Documents;
			std::map<UiExecThread, std::vector<UiEventCall>> m_UiEvents;
			std::mutex m_UiEvents_mutex;
			std::mutex m_Ui_mutex;

			static void EventListener(SDL_Event& event);

	};
}