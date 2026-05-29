#pragma once
#include <imgui/imgui.h>
#include <PaperStocking/debug/debugTool.hpp>

namespace PaperStocking {
    class PerformanceAnalyzer : public BaseDebugTool {
    public:
        PerformanceAnalyzer() = default;
        ~PerformanceAnalyzer() override = default;
        std::string GetName() override {
            return "Performance Analyzer";
        }
        void Render() override {
            ImGui::Begin(GetName().c_str());

            ImGui::End();
        }
    };
}
