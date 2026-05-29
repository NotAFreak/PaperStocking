#pragma once
#include <map>
#include <string>
#include <mutex>
#include <thread>
#include <stack>
namespace PaperStocking {
    enum LogColour : uint8_t {
        Default,
        White,
        Red,
        Yellow,
        Green,
        Blue
    };
    struct Logger {
        public:
            // Avoid using. Pretty please.
            Logger() = default;
            Logger(std::string name, LogColour colour = LogColour::Default);
            ~Logger() = default;

            void Plain(std::string log);
            void PlainError(std::string error);

            void Plain(const char* log);
            void PlainError(const char* error);

            void Info(std::string log, LogColour colour = LogColour::Default);
            void Info(const char* log, LogColour colour = LogColour::Default);

            void Warning(std::string log, LogColour colour = LogColour::Yellow);
            void Warning(const char* log, LogColour colour = LogColour::Yellow);
            
            void Error(std::string log, LogColour colour = LogColour::Red);
            void Error(const char* log, LogColour colour = LogColour::Red);

            void Exception(std::exception exception, LogColour colour = LogColour::Red);

            Logger operator=(const Logger& other) {
                this->m_OwnerThread = other.m_OwnerThread;
                this->m_Name = other.m_Name;
                this->m_NameColor = other.m_NameColor;
            }
        private:
            std::thread::id m_OwnerThread;
            std::string m_Name;
            LogColour m_NameColor;
            std::mutex m_LogMutex;
    };
    struct NestedLogger {
    public:
        NestedLogger(std::string name, LogColour colour = LogColour::Default);
        ~NestedLogger();

        void Begin();
        void End();

        static void Plain(std::string log);
        static void PlainError(std::string error);

        static void Plain(const char* log);
        static void PlainError(const char* error);

        static void Info(std::string log, LogColour colour = LogColour::Default);
        static void Info(const char* log, LogColour colour = LogColour::Default);

        static void Warning(std::string log, LogColour colour = LogColour::Yellow);
        static void Warning(const char* log, LogColour colour = LogColour::Yellow);

        static void Error(std::string log, LogColour colour = LogColour::Red);
        static void Error(const char*, LogColour colour = LogColour::Red);

        static void Exception(std::exception exception, LogColour colour = LogColour::Red);
    private:
        static inline std::map<std::thread::id, std::stack<NestedLogger*>> s_LoggerMap;
        static inline std::mutex s_LoggerMap_mutex;

        Logger* m_logger;
};
}