#pragma once
#include <set>
#include <thread>
#include <vector>

namespace PaperStocking {
	// Only allocate on the stack
	struct ThreadTime {
		public:
			std::string name = "Worker Thread";
			std::thread::id thread;
			std::chrono::high_resolution_clock::time_point startTime;

			std::chrono::high_resolution_clock::time_point lastUpdate;
			std::chrono::high_resolution_clock::duration deltaTime;

			int priority;

			float fDeltaTime;
			double dDeltaTime;
			ThreadTime(std::thread::id thread, std::string name = "Worker Thread", int priority = 0);
			~ThreadTime();
			void Update();
	};
	struct Time {
		public:
			/// @brief Please avoid using this constantly. Use in constructors in order to get refs
			/// @param name
			/// @return
			static ThreadTime* GetThreadTime(std::thread* thread);
			static ThreadTime* GetThreadTime(std::thread::id threadId);
			static ThreadTime* GetThreadTime(std::string threadName);

			static std::vector<ThreadTime*> GetThreads();

			// Done automatically. Don't call unless you know what you're doing.
			static void RegisterThreadTime(ThreadTime* threadTime);
			// Done automatically. Don't call unless you know what you're doing.
			static void UnregisterThreadTime(ThreadTime* threadTime);
		private:
			inline static std::set<ThreadTime*> threads;
			inline static std::mutex threads_mutex;
	};
}