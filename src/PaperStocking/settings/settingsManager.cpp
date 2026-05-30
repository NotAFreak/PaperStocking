#include <PaperStocking/settings/settingsManager.hpp>
#include <filesystem>
using namespace PaperStocking;

SettingsManager::SettingsManager() {
    assert(s_Instance == nullptr);
    s_Instance = this;
    if (!std::filesystem::exists(SETTINGS_PATH)) {
        m_Reader = inih::INIReader();
    } else {
        m_Reader = inih::INIReader(SETTINGS_PATH);
    }
}
SettingsManager::~SettingsManager() {
    assert(s_Instance != nullptr);
    SaveSettings();
    s_Instance = nullptr;
}
