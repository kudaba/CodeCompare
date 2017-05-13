#pragma once

//----------------------------------------------------------------------------------------
// Make sure that there are no custom new/delete operators or custom allocators.
// Custom allocators can affect the true algorithmic performance cost and hide the real memory usage
//----------------------------------------------------------------------------------------
namespace Memory
{
	struct Report
	{
		size_t TotalMemory;
		size_t ActiveAllocations;
		size_t TotalAllocations;

		size_t TrackingMax;
	};

	Report GetGlobalReport();
	Report GetThreadReport();

	void ResetThreadTracking();
}