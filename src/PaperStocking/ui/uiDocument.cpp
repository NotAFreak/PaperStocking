#include <cassert>
#include <PaperStocking/ui/uiManager.hpp>
#include <PaperStocking/ui/uiDocument.hpp>
#include <PaperStocking/assets/assetManager.hpp>

using namespace PaperStocking;

void UiEventListener::ProcessEvent(Rml::Event& event) {
	// prvent event spam
	if (event.GetPhase() != Rml::EventPhase::Target) return;
	UiManager::QueueEvent(m_Callback.m_ExecThread, UiEventCall(m_Callback.m_Callback, &event));
}

UiDocument::UiDocument(Rml::Context* contex, std::string name) {
	this->m_Name = name;
	std::lock_guard lock(m_Document_mutex);
	m_Document = contex->LoadDocument(ASSETS_FOLDER_NAME + "/UI/" + name + ".rml");
	assert(m_Document != nullptr);
	m_Document->Show();
}

UiDocument::~UiDocument() {
	std::lock_guard lock(m_Document_mutex);
	assert(m_Document != nullptr);
	m_Document->Close();
}

void UiDocument::RegisterEvent(std::string name, std::function<void(Rml::Event*)> callback, UiExecThread thread) {
	m_EventCallbacks.emplace(name, UiEventData {
		callback, thread
	});

	// Bind to elements
	int elementCount = m_Document->GetNumChildren();
	for (int i = 0; i < elementCount; ++i) {
		Rml::Element* element = m_Document->GetChild(i);
		const Rml::Variant* eventName = element->GetAttribute("data-event");
		if (!eventName) continue;
		auto it = m_EventCallbacks.find(eventName->Get<Rml::String>());
		if (it == m_EventCallbacks.end()) continue;
		element->AddEventListener("click", new UiEventListener(it->second));
	}
}