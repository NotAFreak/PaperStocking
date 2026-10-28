#pragma once
#include <imgui/imgui.h>
#include <PaperStocking/debug/debugTool.hpp>
#include <PaperStocking/debug/debugUiManager.hpp>

namespace PaperStocking {
    class ToolManager : public BaseDebugTool {
        public:
            ToolManager() = default;
            ~ToolManager() override = default;
            std::string GetName() override {
                return "Tool Manager";
            }
            void Render() override {
                ImGui::Begin(GetName().c_str());
                for (std::pair<const std::string, DebugUiManager::ToolInfo>& info : DebugUiManager::m_DebugToolInfo) {
                    if (strcmp(info.first.c_str(), GetName().c_str()) == 0) continue;
                    ImGui::Checkbox(info.first.c_str(), &info.second.m_Active);
                }
                ImGui::End();
            }
    };
}
