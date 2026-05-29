#pragma once
#include <map>
#include <memory>
#include <SDL3/SDL.h>
#include "debugTool.hpp"
namespace PaperStocking {
    struct DebugUiManager {
        public:
            struct ToolInfo {
                public:
                    bool m_Active = true;
                    ToolInfo() = default;
                    ~ToolInfo() = default;
            };
            bool m_Active = false;
            static inline std::map<std::string, std::unique_ptr<BaseDebugTool>> m_DebugTools;
            static inline std::map<std::string, ToolInfo> m_DebugToolInfo;

            DebugUiManager();
            ~DebugUiManager();

            void Render();
            template <typename T>
            static void AddTool() {
                assert(s_Instance != nullptr);
                std::unique_ptr<T> tool = std::make_unique<T>();
                std::string name = tool->GetName();
                // idk what tf that error is for
                s_Instance->m_DebugTools.emplace(name, std::move(tool));
                s_Instance->m_DebugToolInfo.emplace(name, ToolInfo{});
            }
            static void HideTool(std::string name);
            static void ShowTool(std::string name);
        private:
            static inline DebugUiManager* s_Instance;

            static void EventListener(SDL_Event& event);
    };
}