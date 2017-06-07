#include "Memory.h"
#include <malloc.h>
#include <atomic>
#include <assert.h>

namespace Memory
{
	struct Tracker
	{
		std::atomic_size_t TotalMemory; // only 100% accurate globals

		std::atomic_size_t ActiveAllocations; // only 100% accurate globals
		std::atomic_size_t TotalAllocations;

		std::atomic_size_t TrackingStart;
		std::atomic_size_t TrackingMax;

		void Alloc(size_t bytes)
		{
			TotalMemory += bytes;
			++ActiveAllocations;
			++TotalAllocations;

			if (TotalMemory > TrackingMax)
				TrackingMax = TotalMemory._My_val;
		}

		void Free(size_t bytes)
		{
			assert(TotalMemory >= bytes);
			assert(ActiveAllocations > 0);

			TotalMemory -= bytes;
			--ActiveAllocations;
		}

		void ResetTracking()
		{
			size_t total = TotalMemory;
			TrackingStart = total;
			TrackingMax = total;
		}
	};

	Tracker Global;
	Tracker thread_local Thread;

	Report GetReport(Tracker const& tracker)
	{
		return { tracker.TotalMemory, tracker.ActiveAllocations, tracker.TotalAllocations, tracker.TrackingMax - tracker.TrackingStart };
	}

	Report GetGlobalReport()
	{
		return GetReport(Global);
	}

	Report GetThreadReport()
	{
		return GetReport(Thread);
	}

	void ResetThreadTracking()
	{
		Thread.ResetTracking();
	}

	void* Alloc(size_t bytes)
	{
		Global.Alloc(bytes);
		Thread.Alloc(bytes);
        return malloc(bytes);
	}

	void Free(void* p)
	{
        if (p)
        {
            size_t bytes = _msize(p);
            Global.Free(bytes);
            Thread.Free(bytes);
            free(p);
        }
	}
}

void * operator new(size_t bytes) throw()
{
	return Memory::Alloc(bytes);
}
void * operator new[](size_t bytes) throw()
{
    return Memory::Alloc(bytes);
}
void* operator new  (size_t bytes, const std::nothrow_t&)
{
    return Memory::Alloc(bytes);
}
void* operator new[](size_t bytes, const std::nothrow_t&)
{
    return Memory::Alloc(bytes);
}

void operator delete(void * p) throw()
{
    Memory::Free(p);
}
void operator delete[](void * p) throw()
{
    Memory::Free(p);
}
void operator delete(void * p, const std::nothrow_t&)
{
    Memory::Free(p);
}
void operator delete[](void * p, const std::nothrow_t&)
{
    Memory::Free(p);
}
