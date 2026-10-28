#pragma once

#include <mutex>
#include <functional>
#include <RmlUi/Core.h>
namespace PaperStocking {
	enum UiExecThread : uint8_t {
		Render,
		Game,
		Threadpool
	};
	struct UiEventData {
		std::function<void(Rml::Event* e)> m_Callback;
		UiExecThread m_ExecThread;
	};
	struct UiEventCall {
		public:
			std::function<void(Rml::Event* e)> m_Callback;
			Rml::Event* event;

			UiEventCall(std::function<void(Rml::Event* e)> callback, Rml::Event* event) {
				this->m_Callback = callback;
				this->event = event;
			}
			~UiEventCall() = default;
	};
	class UiEventListener : public Rml::EventListener {
		public:
			UiEventListener(const UiEventData& callback) {
				this->m_Callback = callback;
			}
			void ProcessEvent(Rml::Event& event);
		private:
			UiEventData m_Callback;
	};
	struct UiDocument {
		public:
			std::string m_Name = "???";

			UiDocument() = default;
			UiDocument(Rml::Context* contex, std::string name);
			~UiDocument();

			void RegisterEvent(std::string name, std::function<void(Rml::Event*)>, UiExecThread thread = UiExecThread::Render);
		private:
			Rml::ElementDocument* m_Document = nullptr;
			std::mutex m_Document_mutex;

			std::map<std::string, UiEventData> m_EventCallbacks;
	};
}