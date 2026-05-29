#include "PaperStocking/logger.hpp"

#include <PaperStocking/logger.hpp>
#include <iostream>
#include <cassert>

#define TRUE true
#define FALSE false
// i'm mr frog, this is my show, i eat the bug. i ate the bug. this is the end.
#ifndef LOGGER_ASSERT_THREADS
#define LOGGER_ASSERT_THREADS FALSE
#endif

using namespace PaperStocking;

Logger::Logger(std::string name, LogColour colour) {
    this->m_Name = name;
    this->m_NameColor = colour;
    m_OwnerThread = std::this_thread::get_id();
}
const char* StartColour(LogColour& colour) {
    switch (colour) {
        case LogColour::Default:
            return "";
        case LogColour::White:
            return "\033[37m";
        case LogColour::Red:
            return "\033[31m";
        case LogColour::Yellow:
            return "\033[33m";
        case LogColour::Green:
            return "\033[32m";
        case LogColour::Blue:
            return "\033[34m";
        default:
            return "";
    }
}
const char* EndColour() {
    return "\033[0m";
}
void Logger::Plain(std::string log) {
    #if LOGGER_ASSERT_THREADS
    assert(std::this_thread::get_id() == ownerThread);
    #endif
    std::cout << log << std::endl;
}
void Logger::PlainError(std::string error) {
    #if LOGGER_ASSERT_THREADS
    assert(std::this_thread::get_id() == ownerThread);
    #endif
    std::cout << error << std::endl;
}

void Logger::Plain(const char* log) {
    #if LOGGER_ASSERT_THREADS
    assert(std::this_thread::get_id() == ownerThread);
    #endif
    std::cout << log << std::endl;
}
void Logger::PlainError(const char* error) {
    #if LOGGER_ASSERT_THREADS
    assert(std::this_thread::get_id() == ownerThread);
    #endif
    std::cout << error << std::endl;
}

void Logger::Info(std::string log, LogColour colour) {
    #if LOGGER_ASSERT_THREADS
    assert(std::this_thread::get_id() == ownerThread);
    #endif
    std::lock_guard lock(m_LogMutex);
    std::cout << StartColour(m_NameColor) << m_Name << EndColour() << " - Info:  " << StartColour(colour) << log << EndColour() << std::endl;
}
void Logger::Info(const char* log, LogColour colour) {
    #if LOGGER_ASSERT_THREADS
    assert(std::this_thread::get_id() == ownerThread);
    #endif
    std::lock_guard lock(m_LogMutex);
    std::cout << StartColour(m_NameColor) << m_Name << EndColour() << " - Info:  " << StartColour(colour) << log << EndColour() << std::endl;
}
void Logger::Warning(std::string log, LogColour colour) {
    #if LOGGER_ASSERT_THREADS
    assert(std::this_thread::get_id() == ownerThread);
    #endif
    std::lock_guard lock(m_LogMutex);
    std::cout << StartColour(m_NameColor) << m_Name << EndColour() << " - Warn:  " << StartColour(colour) << log << EndColour() << std::endl;
}
void Logger::Warning(const char* log, LogColour colour) {
    #if LOGGER_ASSERT_THREADS
    assert(std::this_thread::get_id() == ownerThread);
    #endif
    std::lock_guard lock(m_LogMutex);
    std::cout << StartColour(m_NameColor) << m_Name << EndColour() << " - Warn:  " << StartColour(colour) << log << EndColour() << std::endl;
}
void Logger::Error(std::string log, LogColour colour) {
    #if LOGGER_ASSERT_THREADS
    assert(std::this_thread::get_id() == ownerThread);
    #endif
    std::lock_guard lock(m_LogMutex);
    std::cerr << StartColour(m_NameColor) << m_Name << EndColour() << " - Error: " << StartColour(colour) << log << EndColour() << std::endl;
}
void Logger::Error(const char* log, LogColour colour) {
    #if LOGGER_ASSERT_THREADS
    assert(std::this_thread::get_id() == ownerThread);
    #endif
    std::lock_guard lock(m_LogMutex);
    std::cerr << StartColour(m_NameColor) << m_Name << EndColour() << " - Error: " << StartColour(colour) << log << EndColour() << std::endl;
}
void Logger::Exception(std::exception exception, LogColour colour) {
    #if LOGGER_ASSERT_THREADS
    assert(std::this_thread::get_id() == ownerThread);
    #endif

    std::lock_guard lock(m_LogMutex);
    std::cerr << StartColour(m_NameColor) << m_Name << EndColour() << " - Excepion: " << StartColour(colour) << exception.what() << EndColour() << std::endl;
}

//  Nested logger
NestedLogger::NestedLogger(std::string name, LogColour colour) {
    m_logger = new Logger(name, colour);
}
NestedLogger::~NestedLogger() {
    delete m_logger;
}

void NestedLogger::Begin() {
    std::thread::id id = std::this_thread::get_id();
    std::lock_guard lock(s_LoggerMap_mutex);
    if (s_LoggerMap.contains(id)) {
        s_LoggerMap[id].push(this);
    } else {
        std::stack<NestedLogger*> stack;
        s_LoggerMap.emplace(id, stack);
        s_LoggerMap[id].push(this);
    }
}

void NestedLogger::End() {
    std::thread::id id = std::this_thread::get_id();
    std::lock_guard lock(s_LoggerMap_mutex);
    assert(!s_LoggerMap[id].empty());
    s_LoggerMap[id].pop();
}

void NestedLogger::Plain(std::string log) {
    std::thread::id id = std::this_thread::get_id();
    std::lock_guard lock(s_LoggerMap_mutex);
    assert(!s_LoggerMap[id].empty());
    s_LoggerMap[id].top()->m_logger->Plain(log);
}

void NestedLogger::Plain(const char *log) {
    std::thread::id id = std::this_thread::get_id();
    std::lock_guard lock(s_LoggerMap_mutex);
    assert(!s_LoggerMap[id].empty());
    s_LoggerMap[id].top()->m_logger->Plain(log);
}

void NestedLogger::PlainError(std::string error) {
    std::thread::id id = std::this_thread::get_id();
    std::lock_guard lock(s_LoggerMap_mutex);
    assert(!s_LoggerMap[id].empty());
    s_LoggerMap[id].top()->m_logger->PlainError(error);
}

void NestedLogger::PlainError(const char *error) {
    std::thread::id id = std::this_thread::get_id();
    std::lock_guard lock(s_LoggerMap_mutex);
    assert(!s_LoggerMap[id].empty());
    s_LoggerMap[id].top()->m_logger->PlainError(error);
}

void NestedLogger::Info(std::string log, LogColour colour) {
    std::thread::id id = std::this_thread::get_id();
    std::lock_guard lock(s_LoggerMap_mutex);
    assert(!s_LoggerMap[id].empty());
    s_LoggerMap[id].top()->m_logger->Info(log, colour);
}

void NestedLogger::Info(const char *log, LogColour colour) {
    std::thread::id id = std::this_thread::get_id();
    std::lock_guard lock(s_LoggerMap_mutex);
    assert(!s_LoggerMap[id].empty());
    s_LoggerMap[id].top()->m_logger->Info(log, colour);
}

void NestedLogger::Warning(std::string log, LogColour colour) {
    std::thread::id id = std::this_thread::get_id();
    std::lock_guard lock(s_LoggerMap_mutex);
    assert(!s_LoggerMap[id].empty());
    s_LoggerMap[id].top()->m_logger->Warning(log, colour);
}

void NestedLogger::Warning(const char *log, LogColour colour) {
    std::thread::id id = std::this_thread::get_id();
    std::lock_guard lock(s_LoggerMap_mutex);
    assert(!s_LoggerMap[id].empty());
    s_LoggerMap[id].top()->m_logger->Warning(log, colour);
}

void NestedLogger::Error(std::string log, LogColour colour) {
    std::thread::id id = std::this_thread::get_id();
    std::lock_guard lock(s_LoggerMap_mutex);
    assert(!s_LoggerMap[id].empty());
    s_LoggerMap[id].top()->m_logger->Error(log, colour);
}

void NestedLogger::Error(const char* log, LogColour colour) {
    std::thread::id id = std::this_thread::get_id();
    std::lock_guard lock(s_LoggerMap_mutex);
    assert(!s_LoggerMap[id].empty());
    s_LoggerMap[id].top()->m_logger->Error(log, colour);
}

void NestedLogger::Exception(std::exception exception, LogColour colour) {
    std::thread::id id = std::this_thread::get_id();
    std::lock_guard lock(s_LoggerMap_mutex);
    assert(!s_LoggerMap[id].empty());
    s_LoggerMap[id].top()->m_logger->Exception(exception, colour);
}
