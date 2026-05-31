#include <PaperStocking/engine.hpp>
#include <thread>
#include <PaperStocking/thread/threads.hpp>
#include <PaperStocking/debug/timer.hpp>
#include <PaperStocking/graphics/renderer.hpp>
#include <PaperStocking/input/rawInput.hpp>
using namespace PaperStocking;

void Engine::RenderThread() {
	Threads::SetThread(ThreadInfo{
		"Render",1
	});
	RawInput input;
	Renderer renderer;
	RawInput::SubscribeListener(EventListener);
	++m_InitializedThreads;
	while (m_InitializedThreads < c_RequiredThreadCount) { }
	while (s_Run) {
		Timer::Begin("Poll Events");
		input.PollEvents();
		Timer::End();

		Timer::Begin("Rendering");
		renderer.Render();
		Timer::End();

		Timer::FinishThreadFrame();
	}
}

void Engine::GameThread() {
	Threads::SetThread(ThreadInfo{
		"Game",1
	});
	++m_InitializedThreads;
	while (m_InitializedThreads < c_RequiredThreadCount) { }
	while (s_Run) {

	}
}

void Engine::EventListener(SDL_Event& event) {
	switch (event.type) {
		case SDL_EVENT_QUIT:
			s_Run = false;
	}
}
