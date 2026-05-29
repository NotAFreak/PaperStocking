#pragma once
#include <exception>
#include <stdexcept>
#include <string>
namespace PaperStocking {
    class BaseDebugTool {
        public:
            virtual ~BaseDebugTool() = default;
            virtual std::string GetName() {
                throw std::runtime_error("GetName() on DebugTool is unimplemented!");
            }
            virtual void OnShow() { }
            virtual void OnHide() { }
            virtual void Render() { }
    };
}