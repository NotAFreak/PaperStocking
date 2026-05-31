#pragma once

#include <mutex>
#include <RmlUi/Core.h>

namespace PaperStocking {
	struct UiDocument {
		public:
			std::string m_Name = "???";

			UiDocument() = default;
			UiDocument(Rml::Context* contex, std::string name);
			~UiDocument();
		private:
			Rml::ElementDocument* m_Document = nullptr;
			std::mutex m_Document_mutex;
	};
}