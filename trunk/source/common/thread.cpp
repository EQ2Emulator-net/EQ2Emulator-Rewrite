#include "stdafx.h"

#include "thread.h"
#include "Log.h"

using namespace std;

std::atomic<uint32_t> ThreadManager::global_thread_count = ATOMIC_VAR_INIT(0);
thread_local string threadName;

//Not doing anything with the labels yet, but we could
void ThreadManager::EmuThreadInit(const char* ThreadLabel) {
	//srand needs to be ran for every thread on Windows
#ifdef _WIN32
	srand(static_cast<unsigned int>(time(nullptr)));
#endif

	threadName = ThreadLabel;

	global_thread_count.fetch_add(1);

	ostringstream str;
	str << this_thread::get_id();

	LogDebug(LOG_THREAD, 0, "Beginning thread %s (id=%s)", threadName.c_str(), str.str().c_str());
}

void ThreadManager::EmuThreadEnd() {
	global_thread_count.fetch_sub(1);

	ostringstream str;
	str << this_thread::get_id();

	LogDebug(LOG_THREAD, 0, "Ending thread %s (id=%s)", threadName.c_str(), str.str().c_str());
}