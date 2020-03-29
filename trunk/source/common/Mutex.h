#pragma once

#include <atomic>
#include <mutex>
#include <thread>
#include <list>
#include <shared_mutex>

//#define MUTEX_DEBUG

//This is a simple lightweight spinlock with no debug code..use only if you know what you're doing
class SpinLock {
public:
	SpinLock();
	~SpinLock();

	void Lock();
	void Unlock();

private:
	std::atomic_flag lock;
};

/// Mutex class
class Mutex {
public:
	Mutex();
	~Mutex();

	void SetName(const char *name);

	void ReadLock();
	void ReadUnlock();

	void WriteLock();
	void WriteUnlock();

private:
#if defined(EQ2_DEBUG)
	char name[32];
	std::atomic<int32_t> num_readlocks;
	std::thread::id lockingThread;
#ifdef MUTEX_DEBUG
	std::list<std::thread::id> readLockThreads;
	SpinLock m_readLockList;
#endif
#endif

	std::shared_timed_mutex lock;
};

//This is a guaranteed recursive mutex with no debug code.. use only if you know what you're doing
class RecursiveLock {
public:
	RecursiveLock();
	~RecursiveLock();

	void Lock();
	void Unlock();

private:
	//Changing this to a std::recusive_mutex (c++11) just for better portability
	std::recursive_mutex lock;
};

//The following classes allow for exception safe read/write locking our mutexes similar to std::lock_guard
//Normal use would to just create it with a mutex and let the destructor unlock it, but the Lock() and Unlock()
//functions may be used where useful (long functions where you want to unlock early for example)
class ReadLocker {
public:
	explicit ReadLocker(Mutex& lock);

	//Move constructor
	ReadLocker(ReadLocker&& rhs) noexcept {
		lock_object = rhs.lock_object;
		locked = rhs.locked;
		rhs.locked = false;
	}

	ReadLocker(const ReadLocker& rhs) = delete;

	~ReadLocker();
	void Lock();
	void Unlock();
private:
	bool locked;
	Mutex* lock_object;
};

class WriteLocker {
public:
	explicit WriteLocker(Mutex& lock);

	//Move constructor
	WriteLocker(WriteLocker&& rhs) noexcept {
		lock_object = rhs.lock_object;
		locked = rhs.locked;
		rhs.locked = false;
	}

	WriteLocker(const WriteLocker& rhs) = delete;

	~WriteLocker();
	void Lock();
	void Unlock();
private:
	bool locked;
	Mutex* lock_object;
};

class RecursiveLocker {
public:
	explicit RecursiveLocker(RecursiveLock& lock);
	~RecursiveLocker();
	void Lock();
	void Unlock();
private:
	bool locked;
	RecursiveLock* lock_object;
};

class SpinLocker {
public:
	explicit SpinLocker(SpinLock& lock);
	~SpinLocker();
	void Lock();
	void Unlock();
private:
	bool locked;
	SpinLock* lock_object;
};

class QueuedLock {
public:
	QueuedLock();
	~QueuedLock();

	void Lock();
	void Unlock();

private:
	std::atomic<uint32_t> next_lock;
	std::atomic<uint32_t> current_lock;
};