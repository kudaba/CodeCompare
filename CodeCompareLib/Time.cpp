#include "Time.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace Time
{
	struct TimeInfo
	{
		const char* Name;
		const char* Symbol;
		__int64 ConvertFromSeconds;
		int AllowedRange;
	};

	TimeInfo TimeInfos[] =
	{
		{ "Nanoseconds",	"ns",	(__int64)1000000000,					500 },
		{ "Microseconds",	"us",	(__int64)1000000,					500 },
		{ "Milliseconds",	"ms",	(__int64)1000,						500 },
		{ "Seconds",		"s",	(__int64)0,							60 },
		{ "Minutes",		"m",	(__int64)60,							60 },
		{ "Hours",			"h",	(__int64)60 * 60,					12 },
		{ "Days",			"d",	(__int64)60 * 60 * 24,				7 },
		{ "Weeks",			"w",	(__int64)60 * 60 * 24 * 7,			4 },
		{ "Months",			"mo",	(__int64)60 * 60 * 24 * 30,			12 },
		{ "Years",			"y",	(__int64)60 * 60 * 24 * 365,			10 },
		{ "Decades",		"d",	(__int64)60 * 60 * 24 * 365 * 10,	10 },
		{ "Centuries",		"c",	(__int64)60 * 60 * 24 * 365 * 100,	10 },
		{ "Aeons",			"a",	(__int64)60 * 60 * 24 * 365 * 1000,	10 },
	};

	__int64 GetFrequency_Once()
	{
		static_assert(sizeof(TimeInfos) / sizeof(TimeInfos[0]) == (int)TimeScale::Count, "");

		LARGE_INTEGER frequency;
		QueryPerformanceFrequency(&frequency);
		return frequency.QuadPart;
	}

	__int64 Frequency = GetFrequency_Once();

	__int64 GetCurrentTick()
	{
		LARGE_INTEGER time;
		QueryPerformanceCounter(&time);
		return time.QuadPart;
	}

	TimeScale GetTimeScale(__int64 ticks)
	{
		int i = 0;
		for (; i < (int)TimeScale::Seconds; ++i)
		{
			__int64 value = (ticks * TimeInfos[i].ConvertFromSeconds) / Frequency;
			if (value <= TimeInfos[i].AllowedRange)
				return TimeScale(i);
		}

		__int64 seconds = ticks / Frequency;
		if (seconds < TimeInfos[i].AllowedRange)
			return TimeScale::Seconds;

		for (++i; i < (int)TimeScale::Count - 1; ++i)
		{
			__int64 value = seconds / TimeInfos[i].ConvertFromSeconds;
			if (value <= TimeInfos[i].AllowedRange)
				return TimeScale(i);
		}
		return TimeScale(i);
	}

	std::string AutoPrintTime(__int64 ticks)
	{
		TimeScale scale = GetTimeScale(ticks);

		double niceTime;
		if (scale < TimeScale::Seconds)
			niceTime = double(ticks * TimeInfos[(int)scale].ConvertFromSeconds);
		else if (scale > TimeScale::Seconds)
			niceTime = double(ticks / TimeInfos[(int)scale].ConvertFromSeconds);
		else
			niceTime = double(ticks);

		niceTime /= double(Frequency);

		char buffer[128];
		sprintf_s(buffer, "%0.2f%s", (float)niceTime, TimeInfos[(int)scale].Symbol);

		return buffer;
	}
}