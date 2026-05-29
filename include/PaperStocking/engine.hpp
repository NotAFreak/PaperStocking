#pragma once
#include <string>
namespace PaperStocking {
	struct Engine {
		public:
			const std::string c_Version = "1.0.0";
			Engine() = default;
			~Engine() = default;
			void Run(int argc, char* argv[]);

		private:
			static void RenderThread();
			static void GameThread();
	};
}