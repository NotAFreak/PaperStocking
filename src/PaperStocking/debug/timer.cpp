#pragma once
#include <cassert>
#include <PaperStocking/debug/timer.hpp>

using namespace PaperStocking;

void Timer::Begin(std::string name) {
	std::lock_guard lock(s_TimerStack_Mutex);
	auto& stack = s_TimerStack[std::this_thread::get_id()];
	stack.push({
		std::move(name),
		TimerClock::now(),
		{}
	});
}
void Timer::End() {
	const auto threadId = std::this_thread::get_id();
	std::lock_guard lock(s_TimerStack_Mutex);
	auto& stack = s_TimerStack[threadId];
	assert(!stack.empty());
	ActiveTimer active = std::move(stack.top());
	stack.pop();
	TimerRecord record {
		std::move(active.m_Name),
		active.m_StartTime,
		TimerClock::now(),
		std::move(active.m_Children)
	};
	if (!stack.empty()) {
		stack.top().m_Children.push_back(std::move(record));
	} else {
		std::lock_guard frameLock(s_Frames_mutex);
		s_CurrentFrame[threadId].push_back(std::move(record));
	}
}
const std::map<std::thread::id, std::vector<TimerRecord>> Timer::GetTimings() {
	std::lock_guard lock(s_Frames_mutex);
	return s_LastFrame;
}

void Timer::FinishThreadFrame() {
	{
		std::lock_guard lock(s_Frames_mutex);
		s_LastFrame = std::move(s_CurrentFrame);
		s_CurrentFrame.clear();
	} {
		std::lock_guard lock(s_TimerStack_Mutex);
		assert(s_TimerStack[std::this_thread::get_id()].empty());
	}
}