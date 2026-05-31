#include <cassert>
#include <PaperStocking/ui/uiDocument.hpp>
#include <PaperStocking/assets/assetManager.hpp>

using namespace PaperStocking;

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
