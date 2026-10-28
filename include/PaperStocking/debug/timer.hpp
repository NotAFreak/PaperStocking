#pragma once
#include <map>
#include <stack>
#include <mutex>
#include <thread>
#include <chrono>
namespace PaperStocking {
	using TimerClock = std::chrono::steady_clock;
	struct TimerRecord {
		std::string m_Name;
		std::chrono::time_point<TimerClock> m_StartTime;
		std::chrono::time_point<TimerClock> m_EndTime;
		std::vector<TimerRecord> m_Children;
	};
	struct ActiveTimer {
		std::string m_Name;
		std::chrono::time_point<TimerClock> m_StartTime;
		std::vector<TimerRecord> m_Children;
	};

	struct Timer {
		public:
			// Push
			static void Begin(std::string name);
			// Pop
			static void End();

			static void FinishThreadFrame();

			static const std::map<std::thread::id, std::vector<TimerRecord>> GetTimings();
		private:
			// Double buffer timings
			static inline std::map<std::thread::id, std::vector<TimerRecord>> s_LastFrame;
			static inline std::mutex s_Frames_mutex;
			static inline std::map<std::thread::id, std::vector<TimerRecord>> s_CurrentFrame;

			static inline std::map<std::thread::id, std::stack<ActiveTimer>> s_TimerStack;
			static inline std::mutex s_TimerStack_Mutex;
	};
}