#include <thread>
#include <PaperStocking/engine.hpp>
#include <PaperStocking/debug/timer.hpp>
#include <PaperStocking/graphics/renderer.hpp>
#include <PaperStocking/input/rawInput.hpp>
using namespace PaperStocking;

void Engine::RenderThread() {
	ThreadTime time = ThreadTime(std::this_thread::get_id(), "Render", 3);

	Threads::SetThread(ThreadInfo{
		"Render",1
	});
	RawInput input;
	Renderer renderer;
	RawInput::SubscribeListener(EventListener);
	++m_InitializedThreads;
	while (m_InitializedThreads < c_RequiredThreadCount) { }
	time.Update();
	while (s_Run) {
		Timer::Begin("Poll Events");
		input.PollEvents();
		Timer::End();

		Timer::Begin("Rendering");
		renderer.Render();
		Timer::End();

		Timer::FinishThreadFrame();
		time.Update();
	}
}
void Engine::EventListener(SDL_Event& event) {
	switch (event.type) {
		case SDL_EVENT_QUIT:
			s_Run = false;
			break;
	}
}
