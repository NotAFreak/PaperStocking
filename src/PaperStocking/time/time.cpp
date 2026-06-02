#include <algorithm>
#include <PaperStocking/time/time.hpp>

using namespace PaperStocking;

//  Time
ThreadTime* Time::GetThreadTime(std::thread* thread) {
	return GetThreadTime(thread->get_id());
}
ThreadTime* Time::GetThreadTime(std::thread::id threadId) {
	for(ThreadTime* time : threads) {
		if(time->thread == threadId) {
			return time;
		}
	}
	return nullptr;
}
ThreadTime* Time::GetThreadTime(std::string threadName) {
	for(ThreadTime* time : threads) {
		if(time->name == threadName) {
			return time;
		}
	}
	return nullptr;
}

bool comp(ThreadTime* a, ThreadTime* b) {
	return a->priority > b->priority;
}
std::vector<ThreadTime*> Time::GetThreads() {
	std::vector<ThreadTime*> t;
	std::copy(threads.begin(), threads.end(), std::back_inserter(t));

	std::sort(t.begin(), t.end(), comp);
	return t;
}


void Time::RegisterThreadTime(ThreadTime* threadTime) {
	std::lock_guard lock(threads_mutex);
	threads.insert(threadTime);
}
void Time::UnregisterThreadTime(ThreadTime* threadTime) {
	std::lock_guard lock(threads_mutex);
	threads.erase(threadTime);
}

// Thread Time

ThreadTime::ThreadTime(std::thread::id thread, std::string name, int priority) {
	this->name = name;
	this->thread = thread;
	this->startTime = std::chrono::high_resolution_clock::now();
	this->lastUpdate = startTime;
	this->priority = priority;

	Time::RegisterThreadTime(this);
}
ThreadTime::~ThreadTime() {
	Time::UnregisterThreadTime(this);
}
void ThreadTime::Update() {
	std::chrono::high_resolution_clock::time_point last = lastUpdate;
	lastUpdate = std::chrono::high_resolution_clock::now();

	deltaTime = lastUpdate - last;

	dDeltaTime = (double)((double)deltaTime.count() / (double)1000000000);
	fDeltaTime = (float)dDeltaTime;
}