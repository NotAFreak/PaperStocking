#include <cassert>
#include <glm/glm.hpp>
#include <PaperStocking/assets/assetManager.hpp>
#include <PaperStocking/graphics/renderer.hpp>
#include <PaperStocking/ui/uiManager.hpp>

#include <RmlUi_Backend.h>
#include <PaperStocking/input/rawInput.hpp>
#include <RmlUi/Debugger/Debugger.h>

using namespace PaperStocking;
UiManager::UiManager() {
	assert(s_Instance == nullptr);
	s_Instance = this;

	RawInput::SubscribeListener(EventListener);
}

UiManager::~UiManager() {
	assert(s_Instance != nullptr);
	s_Instance = nullptr;
}

void LoadFontsFromFolder(std::filesystem::path path) {
	std::filesystem::directory_iterator dir(path);
	for (std::filesystem::directory_entry entry : dir) {
		if (entry.is_directory()) {
			LoadFontsFromFolder(entry.path());
		} else {
			Rml::LoadFontFace(entry.path());
		}
	}
}
void UiManager::Initialize() {
	assert(s_Instance != nullptr);
	glm::uvec2 resolution = Renderer::GetOutputResolution();
	s_Instance->m_Context = Rml::CreateContext("main", Rml::Vector2i(resolution.x, resolution.y));
	Rml::Debugger::Initialise(s_Instance->m_Context);
	// Load Fonts
	std::string fontFolder = ASSETS_FOLDER_NAME + "/UI/Fonts";
	LoadFontsFromFolder(std::filesystem::absolute(fontFolder));
	RawInput::SubscribeListener(EventListener);
}

void UiManager::Shutdown() {
	m_Documents.clear();
	Rml::RemoveContext("main");
}

void UiManager::Update() {
	std::lock_guard lock(s_Instance->m_Ui_mutex);
	m_Context->Update();
}

void UiManager::Render() {
	std::lock_guard lock(s_Instance->m_Ui_mutex);
	m_Context->Render();
}

void UiManager::EventListener(SDL_Event& event) {
	assert(s_Instance != nullptr);
	if (event.type == SDL_EVENT_KEY_DOWN) {
		if (event.key.key == SDLK_F8) {
			s_Instance->m_UiDebugVisible = !s_Instance->m_UiDebugVisible;
			Rml::Debugger::SetVisible(s_Instance->m_UiDebugVisible);
		}
	}
	RmlSDL::InputEventHandler(s_Instance->m_Context, Renderer::s_Window, event);
}

std::weak_ptr<UiDocument> UiManager::LoadDocument(std::string name) {
	assert(s_Instance != nullptr);
	std::lock_guard lock(s_Instance->m_Ui_mutex);
	std::shared_ptr<UiDocument> doc = std::make_shared<UiDocument>(s_Instance->m_Context, name);
	s_Instance->m_Documents.emplace(name, doc);
	return std::weak_ptr<UiDocument>(doc);
}

void UiManager::UnloadDocument(std::string name) {
	assert(s_Instance != nullptr);
	std::lock_guard lock(s_Instance->m_Ui_mutex);
	std::map<std::string, std::shared_ptr<UiDocument>>::iterator it = s_Instance->m_Documents.find(name);
	s_Instance->m_Documents.erase(it);
}

void UiManager::UnloadDocument(std::weak_ptr<UiDocument> doc) {
	assert(s_Instance != nullptr);
	std::lock_guard lock(s_Instance->m_Ui_mutex);
	std::map<std::string, std::shared_ptr<UiDocument>>::iterator it = s_Instance->m_Documents.find(doc.lock()->m_Name);
	s_Instance->m_Documents.erase(it);
}

void UiManager::QueueEvent(UiExecThread thread, UiEventCall call) {
	assert(s_Instance != nullptr);
	std::lock_guard lock(s_Instance->m_UiEvents_mutex);
	if (!s_Instance->m_UiEvents.contains(thread)) {
		s_Instance->m_UiEvents.emplace(thread, std::vector<UiEventCall>());
	}
	s_Instance->m_UiEvents[thread].push_back(call);
}

void UiManager::ExecuteAllEvents(UiExecThread thread) {
	assert(s_Instance != nullptr);
	std::lock_guard lock(s_Instance->m_UiEvents_mutex);
	for (auto& ventData : s_Instance->m_UiEvents[thread]) {
		ventData.m_Callback(ventData.event);
	}
	s_Instance->m_UiEvents[thread].clear();
}
