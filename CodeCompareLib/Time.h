#pragma once

#include <string>

//----------------------------------------------------------------------------------------
// Utilities to time operations and print results in a nice way
//----------------------------------------------------------------------------------------
namespace Time
{
	enum class TimeScale
	{
		Nano,
		Micro,
		Milli,
		Seconds,
		Minutes,
		Hours,
		Days,
		Weeks,
		Months,
		Years,
		Decades,
		Centuries,
		Aeons,

		Count
	};

	__int64 GetCurrentTick();

	TimeScale GetTimeScale(__int64 ticks);

	std::string AutoPrintTime(__int64 ticke);
}