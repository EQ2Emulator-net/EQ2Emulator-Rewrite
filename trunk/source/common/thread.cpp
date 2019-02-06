/**
* @file Thread.cpp
*
* @brief A thread implementation.
*
* @details This threading implementation is cross-platform and hides a lot of
* the typical things you need to do when starting a thread on either platform.
* The thread will clean itself up after it finishes running so there's no need
* for the programmer to worry about it.
* TODO: In debug mode, track treads by name?
*
* @author Scatman
* @date 08/31/2013
*
* @section License
* VGOEmulator:  Vanguard: Saga of Heroes Server Emulator
* Copyright (C) 2014  VGOEmulator Development Team (http://vgoemulator.net)
*
* This file is part of VGOEmulator.
*
* VGOEmulator is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* VGOEmulator is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with VGOEmulator.  If not, see <http://www.gnu.org/licenses/>.
*/

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