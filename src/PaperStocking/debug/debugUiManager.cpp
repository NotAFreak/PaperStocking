#include <cassert>
#include <imgui/imgui.h>
#include <PaperStocking/input/rawInput.hpp>
#include <PaperStocking/debug/debugUiManager.hpp>

#include <PaperStocking/engine.hpp>
#include <PaperStocking/debug/tools/tools.hpp>
using namespace PaperStocking;

DebugUiManager::DebugUiManager() {
    assert(s_Instance == nullptr);
    s_Instance = this;

    RawInput::SubscribeListener(EventListener);
    AddTool<ToolManager>();
    AddTool<PerformanceAnalyzer>();
}

DebugUiManager::~DebugUiManager() {
    assert(s_Instance != nullptr);
    s_Instance = nullptr;
    RawInput::UnsubscribeListener(EventListener);
}
void DebugUiManager::Render() {
    if (!m_Active) return;
    if(ImGui::BeginMainMenuBar()) {
        ImGui::Text(("PaperStocking - v" + std::string(Engine::c_Version)).c_str());
        ImGui::EndMainMenuBar();
    }

    for (std::pair<const std::string, std::unique_ptr<BaseDebugTool>>& tool : m_DebugTools) {
        assert(m_DebugToolInfo.contains(tool.first));
        ToolInfo& info = m_DebugToolInfo[tool.first];
        if (!info.m_Active) continue;
        tool.second->Render();
    }
}
void DebugUiManager::EventListener(SDL_Event& event) {
    assert(s_Instance != nullptr);
    if (event.type == SDL_EVENT_KEY_DOWN) {
        if (event.key.key == SDLK_F7) {
            s_Instance->m_Active = !s_Instance->m_Active;
        }
    }
}

void DebugUiManager::HideTool(std::string name) {
    assert(s_Instance != nullptr);
    assert(s_Instance->m_DebugTools.contains(name));
    assert(s_Instance->m_DebugToolInfo.contains(name));
    m_DebugToolInfo[name].m_Active = false;
}

void DebugUiManager::ShowTool(std::string name) {
    assert(s_Instance != nullptr);
    assert(s_Instance->m_DebugTools.contains(name));
    assert(s_Instance->m_DebugToolInfo.contains(name));
    m_DebugToolInfo[name].m_Active = true;
}
