#include <PaperStocking/settings/settingsManager.hpp>

using namespace PaperStocking;

SettingsManager::SettingsManager() {
    m_Reader = inih::INIReader(SETTINGS_PATH);
}