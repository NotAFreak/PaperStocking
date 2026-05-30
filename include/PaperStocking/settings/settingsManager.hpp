#pragma once

#include <ini.h>
#include <string>
#include <cassert>
#include <thread>

#ifndef SETTINGS_PATH
#define SETTINGS_PATH "settings.ini"
#endif
namespace PaperStocking {
    struct SettingsManager {
        public:
            SettingsManager();
            ~SettingsManager();

            template<typename T>
            static T GetSetting(const std::string& section, const std::string& setting, T defaultValue) {
                assert(s_Instance != nullptr);
                std::lock_guard lock(ioMutex);
                if (!s_Instance->m_Reader.Sections().contains(section)) {
                    s_Instance->m_Reader.InsertEntry(section, setting, defaultValue);
                }
                if (!s_Instance->m_Reader.Keys(section).contains(setting)) {
                    s_Instance->m_Reader.InsertEntry(section, setting, defaultValue);
                    return defaultValue;
                }
                return s_Instance->m_Reader.Get<T>(section, setting);
            }

            template<typename T>
            static void SetSetting(const std::string& section, const std::string& setting, const T& value, bool save = true) {
                assert(s_Instance);
                std::lock_guard lock(ioMutex);
                if (!s_Instance->m_Reader.Sections().contains(section) || !s_Instance->m_Reader.Keys(section).contains(setting)) {
                    s_Instance->m_Reader.InsertEntry(section, setting, value);
                } else {
                    s_Instance->m_Reader.UpdateEntry(section, setting, value);
                }
                if (save) {
                    SaveSettings();
                }
            }

            static void SaveSettings() {
                assert(s_Instance != nullptr);
                std::lock_guard lock(ioMutex);
                s_Instance->m_Writer.write(SETTINGS_PATH, s_Instance->m_Reader, true);
            }
        private:
            static inline SettingsManager* s_Instance;
            static inline std::mutex ioMutex;
            inih::INIReader m_Reader;
            inih::INIWriter m_Writer;
    };
}