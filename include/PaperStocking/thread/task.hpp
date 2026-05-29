#pragma once
#include <memory>
namespace PaperStocking {
	struct Task {
		public:
			void(*task);
			void(*onComplete);

			void Scehdule();

			static std::shared_ptr<Task> CreateTask(void* task);
		private:
			Task();
			~Task();
	};
}