#include <PaperStocking/engine.hpp>
#include <tclap/CmdLine.h>
#include <thread>
#include "PaperStocking/logger.hpp"

#include <PaperStocking/graphics/renderer.hpp>
#include <PaperStocking/input/rawInput.hpp>
using namespace PaperStocking;
void Engine::Run(int argc, char* argv[]) {
	PaperStocking::Logger logger("PaperStocking", LogColour::Blue);
	try {
		TCLAP::CmdLine cmd("This is a PaperStocking game", ' ', c_Version);
		cmd.parse(argc, argv);
	} catch (TCLAP::ArgException& ex) {
		logger.Error("Error parsing args: " + std::string(ex.what()));
	}
	logger.Info("Starting engine, version " + c_Version);
	std::thread gameThread(GameThread);
	RenderThread();
	gameThread.join();
	logger.Info("Stopped engine!");
}

void Engine::RenderThread() {
	RawInput input;
	Renderer renderer;
	RawInput::SubscribeListener(EventListener);
	while (s_Run){
		input.PollEvents();
		renderer.Render();
	}
}

void Engine::GameThread() {

}

void Engine::EventListener(SDL_Event& event) {
	switch (event.type) {
		case SDL_EVENT_QUIT:
			s_Run = false;
	}
}
