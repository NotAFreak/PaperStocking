#include <cassert>
#include <PaperStocking/thread/threads.hpp>
using namespace PaperStocking;

void Threads::SetThread(const std::string& name) {
	std::thread::id thread = std::this_thread::get_id();
	std::lock_guard lock(s_Names_mutex);
	if (s_Names.contains(thread)) {
		s_Names.emplace(thread, ThreadInfo {
			name
		});
	} else {
		s_Names[thread].name = name;
	}
}

void Threads::SetThread(std::thread::id thread, const std::string& name) {
	std::lock_guard lock(s_Names_mutex);
	if (s_Names.contains(thread)) {
		s_Names.emplace(thread, ThreadInfo {
			name
		});
	} else {
		s_Names[thread].name = name;
	}
}

void Threads::SetThread(ThreadInfo info) {
	std::thread::id thread = std::this_thread::get_id();
	std::lock_guard lock(s_Names_mutex);
	if (s_Names.contains(thread)) {
		s_Names.emplace(thread, info);
	} else {
		s_Names[thread] = info;
	}
}

void Threads::SetThread(std::thread::id thread, ThreadInfo info) {
	std::lock_guard lock(s_Names_mutex);
	if (s_Names.contains(thread)) {
		s_Names.emplace(thread, info);
	} else {
		s_Names[thread] = info;
	}
}

ThreadInfo Threads::GetThread() {
	std::lock_guard lock(s_Names_mutex);
	std::thread::id thread = std::this_thread::get_id();
	assert(s_Names.contains(thread));
	return s_Names[thread];
}

ThreadInfo Threads::GetThread(const std::thread::id thread) {
	std::lock_guard lock(s_Names_mutex);
	assert(s_Names.contains(thread));
	return s_Names[thread];
}
