#pragma once
#include <string>
#include <atomic>
#include <SDL3/SDL.h>
#include <tclap/CmdLine.h>
#include <PaperStocking/logger.hpp>
#include <PaperStocking/thread/threads.hpp>
#include <PaperStocking/game/gameManager.hpp>
#include <PaperStocking/settings/settingsManager.hpp>
namespace PaperStocking {
	struct EngineStartInfo {
		std::string m_ApplicationName;
		std::string m_ApplicationVersion;
	};
	struct Engine {
		public:
			const static inline std::string c_Version = "1.0.0";
			static inline std::atomic_bool s_Run = true;
			static inline EngineStartInfo info;

			Engine() = default;
			~Engine() = default;
			template <typename EntrySystem>
			void Run(int argc, char* argv[], EngineStartInfo& info) {
				PaperStocking::Logger logger("PaperStocking", LogColour::Blue);
				this->info = info;
				try {
					TCLAP::CmdLine cmd("This is a PaperStocking game", ' ', c_Version);
					cmd.parse(argc, argv);
				} catch (TCLAP::ArgException& ex) {
					logger.Error("Error parsing args: " + std::string(ex.what()));
				}
				logger.Info("Starting engine, version " + c_Version);
				std::thread gameThread(GameThread<EntrySystem>);
				RenderThread();
				gameThread.join();
				logger.Info("Stopped engine!");
			}
		private:
			static void RenderThread();

			template<typename EntrySystem>
			static void GameThread() {
				Threads::SetThread(ThreadInfo{
					"Game",1
				});
				NestedLogger logger = NestedLogger("Game Manager");
				++m_InitializedThreads;
				while (m_InitializedThreads < c_RequiredThreadCount) { }
				logger.Begin();
				GameManager gameManager;
				GameManager::AddSystem<EntrySystem>();
				while (s_Run) {
					gameManager.Update();
				}
				logger.End();
			}

			static void EventListener(SDL_Event& event);

			SettingsManager m_SettingsManager;
			static inline std::atomic_uint8_t m_InitializedThreads = 0;
			static constexpr uint8_t c_RequiredThreadCount = 2;
	};
}
