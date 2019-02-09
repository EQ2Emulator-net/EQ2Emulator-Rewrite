#include "stdafx.h"

#if defined(EQ2_DEBUG)
#include "Log.h"
#include "Util.h"
#include "string.h"
#endif

#include "Mutex.h"

#define MUTEX_TIMEOUT_SECONDS 10

using namespace std;
const chrono::seconds MUTEX_TIMEOUT(MUTEX_TIMEOUT_SECONDS);

/**
* @brief Constructor.
*
* @param name The name of the mutex.
*/
Mutex::Mutex() {
#if defined(EQ2_DEBUG)
	memset(name, 0, sizeof(name));
	num_readlocks = 0;
#endif

}

/**
* @brief Destructor.
*/
Mutex::~Mutex() {
#ifdef EQ2_DEBUG
	static const thread::id defaultThreadId = thread::id();
	assert(num_readlocks == 0 && lockingThread == defaultThreadId);
#endif
}

void Mutex::SetName(const char *name) {
#if defined(EQ2_DEBUG)
	strlcpy(this->name, name, sizeof(this->name));
#endif
}

void Mutex::ReadLock() {
	while (!lock.try_lock_shared_for(MUTEX_TIMEOUT)) {
#ifdef EQ2_DEBUG
		LogError(LOG_MUTEX, 0, "Timeout trying to acquire mutex %s\'s read lock after %u seconds", name, MUTEX_TIMEOUT_SECONDS);
#endif
	}

#ifdef EQ2_DEBUG
	num_readlocks.fetch_add(1);

#ifdef MUTEX_DEBUG
	m_readLockList.Lock();
	readLockThreads.emplace_back(this_thread::get_id());
	m_readLockList.Unlock();
#endif
#endif
}

void Mutex::ReadUnlock() {
#ifdef EQ2_DEBUG
	assert(num_readlocks.fetch_sub(1) > 0);
#endif

#ifdef MUTEX_DEBUG
	m_readLockList.Lock();
	thread::id id = this_thread::get_id();
	for (auto itr = readLockThreads.begin(); itr != readLockThreads.end(); itr++) {
		if (*itr == id) {
			readLockThreads.erase(itr);
			break;
		}
	}
	m_readLockList.Unlock();
#endif

	lock.unlock_shared();
}

void Mutex::WriteLock() {
	while (!lock.try_lock_for(MUTEX_TIMEOUT)) {
#ifdef EQ2_DEBUG
		LogError(LOG_MUTEX, 0, "Timeout trying to acquire mutex %s\'s write lock lock after %u seconds", name, MUTEX_TIMEOUT_SECONDS);
#endif
	}

#ifdef EQ2_DEBUG
	lockingThread = this_thread::get_id();
#endif
}

/**
* @brief Releases the mutex's write lock.
*
* @details TODO
*/
void Mutex::WriteUnlock() {
#ifdef EQ2_DEBUG
	assert(lockingThread == this_thread::get_id());
	lockingThread = thread::id();
#endif

	lock.unlock();
}

RecursiveLock::RecursiveLock() {
}

RecursiveLock::~RecursiveLock() {
}

void RecursiveLock::Lock() {
	lock.lock();
}

void RecursiveLock::Unlock() {
	lock.unlock();
}

SpinLock::SpinLock() {
	lock.clear();
}

SpinLock::~SpinLock() {
}

void SpinLock::Lock() {
	while (lock.test_and_set()) {
		//spin...
	}
}

void SpinLock::Unlock() {
	lock.clear();
}

ReadLocker::ReadLocker(Mutex& lock) :
	locked(true),
	lock_object(&lock)
{
	lock_object->ReadLock();
}

ReadLocker::~ReadLocker() {
	if (locked) lock_object->ReadUnlock();
}

void ReadLocker::Lock() {
	if (!locked) {
		lock_object->ReadLock();
		locked = true;
	}
}

void ReadLocker::Unlock() {
	if (locked) {
		lock_object->ReadUnlock();
		locked = false;
	}
}

WriteLocker::WriteLocker(Mutex& lock) :
	locked(true),
	lock_object(&lock)
{
	lock_object->WriteLock();
}

WriteLocker::~WriteLocker() {
	if (locked) lock_object->WriteUnlock();
}

void WriteLocker::Lock() {
	if (!locked) {
		lock_object->WriteLock();
		locked = true;
	}
}

void WriteLocker::Unlock() {
	if (locked) {
		lock_object->WriteUnlock();
		locked = false;
	}
}

RecursiveLocker::RecursiveLocker(RecursiveLock& lock) :
	lock_object(&lock),
	locked(true)
{
	lock_object->Lock();
}

RecursiveLocker::~RecursiveLocker() {
	if (locked) lock_object->Unlock();
}

void RecursiveLocker::Lock() {
	if (!locked) lock_object->Lock();
}

void RecursiveLocker::Unlock() {
	if (locked) lock_object->Unlock();
}

SpinLocker::SpinLocker(SpinLock& lock) :
	lock_object(&lock),
	locked(true)
{
	lock_object->Lock();
}

SpinLocker::~SpinLocker() {
	if (locked) lock_object->Unlock();
}

void SpinLocker::Lock() {
	if (!locked) lock_object->Lock();
}

void SpinLocker::Unlock() {
	if (locked) lock_object->Unlock();
}