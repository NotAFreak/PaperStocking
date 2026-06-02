#pragma once
#include <map>
#include <chrono>
#include <thread>
#include <string>
namespace PaperStocking {
	using GameClock = std::chrono::steady_clock;
	struct ThreadInfo {
		std::string name = "???";
		int priority = 0;
	};
	struct Threads {
		public:
			static ThreadInfo GetThread();
			static ThreadInfo GetThread(std::thread::id thread);

			static void SetThread(const std::string& name);
			static void SetThread(std::thread::id thread, const std::string& name);
			static void SetThread(ThreadInfo info);
			static void SetThread(std::thread::id thread, ThreadInfo info);
		private:
			static inline std::map<std::thread::id, ThreadInfo> s_Names;
			static inline std::mutex s_Names_mutex;
	};
}