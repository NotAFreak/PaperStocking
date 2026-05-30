#pragma once
#include <imgui/imgui.h>
#include <PaperStocking/debug/timer.hpp>
#include <PaperStocking/thread/threads.hpp>
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

                // Frame timing tree
                auto timings = Timer::GetTimings();
                {
                    ImGui::BeginGroup();
                    for (std::pair<std::thread::id, std::vector<TimerRecord>> pair : timings) {
                        if (ImGui::CollapsingHeader(Threads::GetThread().name.c_str())) {
                            for (TimerRecord& timing : pair.second) {
                                DisplayRecord(timing);
                            }
                        }

                    }
                    ImGui::EndGroup();
                }
                ImGui::End();
            }
        private:
            void DisplayRecord(TimerRecord& record) {
                std::chrono::microseconds microseconds = std::chrono::duration_cast<std::chrono::microseconds>(record.m_EndTime - record.m_StartTime);
                std::string timeDisplay =
                    std::to_string(1.0f / (static_cast<float>(microseconds.count()) / 1000000.0f)) + "Hz " +
                    std::to_string(static_cast<float>(microseconds.count()) / 1000.0f) + "ms";
                if (record.m_Children.empty()) {
                    ImGui::TreeNodeEx((record.m_Name + " " + timeDisplay).c_str(), ImGuiTreeNodeFlags_Leaf);
                    ImGui::TreePop();
                    return;
                }
                if (ImGui::TreeNode((record.m_Name + " " + timeDisplay).c_str())) {
                    for (auto& timing : record.m_Children) {
                        DisplayRecord(timing);
                    }
                    ImGui::TreePop();
                }
            }
    };
}
