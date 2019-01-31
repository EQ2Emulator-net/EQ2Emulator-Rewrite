#pragma once

#include <thread>
#include <type_traits>
#include <atomic>
#include <stdint.h>
#include <future>

class ThreadManager {
private:
	static void EmuThreadInit(const char* ThreadLabel);
	static void EmuThreadEnd();

	template<typename T>
	static typename std::result_of<T()>::type ThreadStartHelper(const char* ThreadLabel, T func) {
		//We can't return a variable if the return type is void due to a compilation error
		//So we need to return from the function call directly, use ThreadInit's destructor to call EmuThreadEnd
		class ThreadInit {
			friend class ThreadManager;
		public:
			ThreadInit(const char* label) {
				ThreadManager::EmuThreadInit(label);
			}

			~ThreadInit() {
				ThreadManager::EmuThreadEnd();
			}
		};

		ThreadInit init(ThreadLabel);
		return func();
	}

public:
	//This function returns a std::thread object which must have either the detach() or join() function called on it before destruction
	//Pass in the thread name and a std::bind object that will be called on the new thread as the args
	template<typename T>
	static std::thread ThreadStart(const char* ThreadLabel, T func) {
		static_assert(std::is_bind_expression<T>::value, "ThreadManager::ThreadStart must use a std::bind object as \"func\"");
		return std::thread(&ThreadManager::ThreadStartHelper<T>, ThreadLabel, func);
	}

	template <typename T>
	static std::future<typename std::result_of<T()>::type> ThreadStartWithFuture(const char* ThreadLabel, T func) {
		static_assert(std::is_bind_expression<T>::value, "ThreadManager::ThreadStartWithFuture must use a std::bind object as \"func\"");
		return std::async(std::launch::async, &ThreadManager::ThreadStartHelper<T>, ThreadLabel, func);
	}

	static std::atomic<uint32_t> global_thread_count;
};
