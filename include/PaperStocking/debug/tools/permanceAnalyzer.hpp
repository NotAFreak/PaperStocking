#pragma once
#include <imgui/imgui.h>
#include <PaperStocking/debug/timer.hpp>
#include <PaperStocking/thread/threads.hpp>
#include <PaperStocking/debug/debugTool.hpp>
namespace PaperStocking {
    class PerformanceAnalyzer : public BaseDebugTool {
        private:
            bool LabeledCollapsingHeader(const std::string& name, const std::string& label) {
                bool open = ImGui::CollapsingHeader(name.c_str());
                ImGui::SameLine();
                ImGui::Text(label.c_str());
                return open;
            }
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
                        // if (ImGui::CollapsingHeader(Threads::GetThread(pair.first).name.c_str())) {
                        ThreadTime* time = Time::GetThreadTime(pair.first);
                        if (time == nullptr) {
                            ImGui::EndGroup();
                            ImGui::End();
                            return;
                        }
                        if (LabeledCollapsingHeader(Threads::GetThread(pair.first).name, std::to_string(1.0f / time->fDeltaTime) + "Hz")) {
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
            bool LabeledTreeNode(const std::string& name, const std::string& label) {
                bool open = ImGui::TreeNode(name.c_str());
                ImGui::SameLine();
                ImGui::Text(label.c_str());
                return open;
            }
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
                if (LabeledTreeNode(record.m_Name, timeDisplay)) {
                    for (auto& timing : record.m_Children) {
                        DisplayRecord(timing);
                    }
                    ImGui::TreePop();
                }
            }
    };
}
