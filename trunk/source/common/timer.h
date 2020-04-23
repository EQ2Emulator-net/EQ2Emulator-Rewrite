/*  
    EQ2Emulator:  Everquest II Server Emulator
    Copyright (C) 2007  EQ2EMulator Development Team (http://www.eq2emulator.net)

    This file is part of EQ2Emulator.

    EQ2Emulator is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    EQ2Emulator is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with EQ2Emulator.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once

#include <cstdint>

class Timer
{
public:
	Timer();
	Timer(uint32_t timer_time, bool iUseAcurateTiming = false);
	Timer(uint32_t start, uint32_t timer, bool iUseAcurateTiming);
	~Timer() { }

	bool Check(bool iReset = true);
	void Enable();
	void Disable();
	void Start(uint32_t set_timer_time=0, bool ChangeResetTimer = true);
	void SetTimer(uint32_t set_timer_time=0);
	uint32_t GetRemainingTime();
	uint32_t GetElapsedTime();
	inline const uint32_t& GetTimerTime()		{ return timer_time; }
	inline const uint32_t& GetSetAtTrigger()	{ return set_at_trigger; }
	void Trigger();
	void SetAtTrigger(uint32_t set_at_trigger, bool iEnableIfDisabled = false);

	inline bool Enabled() { return enabled; }
	inline uint32_t GetStartTime() { return(start_time); }
	inline uint32_t GetDuration() { return(timer_time); }

	static void SetCurrentTime();
	static uint32_t GetServerTime();
	static uint32_t GetUnixTimeStamp();

private:
	static std::atomic<uint32_t> current_time;
	uint32_t	start_time;
	uint32_t	timer_time;
	bool	enabled;
	uint32_t	set_at_trigger;

	// Tells the timer to be more acurate about happening every X ms.
	// Instead of Check() setting the start_time = now,
	// it it sets it to start_time += timer_time
	bool	pUseAcurateTiming;

//	static int32 current_time;
//	static int32 last_time;
};