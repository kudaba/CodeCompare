#include "PrintHelpers.h"
#include "Memory.h"
#include "Time.h"
#include "TestHarness.h"

namespace Bootstrap
{
	//----------------------------------------------------------------------------------------
	// Helper to print pass results
	//----------------------------------------------------------------------------------------
	void PrintPass(const char* name, TestResults::PassResults const& pass)
	{
		cout << name << endl;

		for (auto const& test : pass.Results)
		{
			cout << test.first << endl;

			cout << "Perf: ";
			PrintList(test.second.Results, cout, [&](auto& item) { cout << item.Performance; });
			cout << endl;

			cout << "Mem: ";
			PrintList(test.second.Results, cout, [&](auto& item) { cout << item.MemoryUsage; });
			cout << endl;

			cout << "Custom: ";
			PrintList(test.second.Results, cout, [&](auto& item) { cout << item.CustomResult; });
			cout << endl;
		}
		cout << endl;
	}

	//----------------------------------------------------------------------------------------
	// Run tests and optionally print results to console
	//----------------------------------------------------------------------------------------
	void executeTests(TestHarness const& harness)
	{
		auto suite = harness.CreateTest();
		auto results = suite->RunTests();

		if (harness.Verbose)
		{
			cout << endl << endl;

			for (auto const& pass : results->Passes)
			{
				PrintPass(pass.first.c_str(), pass.second);
			}

			PrintPass("Summary", results->Summary);
		}

		harness.PrintTest(*results);
	}

	//----------------------------------------------------------------------------------------
	// Setup conditions for memory leak detection and overall performance test
	//----------------------------------------------------------------------------------------
	void RunTests(TestHarness const& harness)
	{
		assert(Memory::GetGlobalReport().TotalMemory == 0);
		auto begin = Time::GetCurrentTick();
		executeTests(harness);
		auto end = Time::GetCurrentTick();

		auto report = Memory::GetGlobalReport();
		assert(report.TotalMemory == 0);

		cout << endl;

		if (report.TotalMemory > 0)
			cout << "Leaked Memory: " << report.TotalMemory << endl;

		cout << "Maximum Memory Used: " << report.TrackingMax << endl;
		cout << "Total Allocations: " << report.TotalAllocations << endl;
		cout << "Total Execution time: " << Time::AutoPrintTime(end - begin).c_str() << endl;
	}
}