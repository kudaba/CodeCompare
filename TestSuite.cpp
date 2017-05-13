#include "TestSuite.h"
#include "Time.h"
#include "Memory.h"
#include <assert.h>

unique_ptr<TestResults> TestSuite::RunTests() const
{
	TestResults* testResults = new TestResults();
	auto const numParams = Parameters.size();

	testResults->Parameters.reserve(Parameters.size());
	for (auto const& p : Parameters)
		testResults->Parameters.push_back(p->ToString());

	cout << "Beginning " << GetName() << endl;

	for (auto const& test : Tests)
	{
		cout << "  Running Test: " << test->GetName() << endl;

		auto& summary = testResults->Summary[test->GetName()];
		summary.resize(numParams);

		for (auto const& pass : test->GetPasses())
		{
			cout << "    Running Pass: " << pass.first << endl;

			auto& passResults = testResults->Passes[pass.first];
			auto& results = passResults[test->GetName()];
			results.reserve(numParams);

			bool first = true;

			TestWeight weight;
			auto itr = PassWeights.find(pass.first);
			if (itr != PassWeights.end())
				weight = itr->second;

			for (int i = 0; i < numParams; ++i)
			{
				auto const& parameter = Parameters[i];

				if (!first)
					cout << ", ";

				cout << parameter->ToString();
				first = false;

				TestResult result;

				Memory::ResetThreadTracking();
				Memory::Report membegin = Memory::GetThreadReport();
				__int64 begin = Time::GetCurrentTick();

				result.CustomResult = pass.second(parameter.get());

				__int64 end = Time::GetCurrentTick();
				Memory::Report memend = Memory::GetThreadReport();

				assert(memend.TotalMemory == membegin.TotalMemory);
				assert(memend.TotalAllocations == membegin.TotalAllocations);

				result.Performance = end - begin;
				result.MemoryUsage = memend.TrackingMax;

				results.push_back(std::move(result));

				summary[i].Performance += __int64(result.Performance * weight.Performance);
				summary[i].MemoryUsage += __int64(result.MemoryUsage * weight.MemoryUsage);
				summary[i].CustomResult += __int64(result.CustomResult * weight.CustomResult);
			}

			cout << endl;
		}

		for (int i = 0; i < numParams; ++i)
		{
			summary[i].Performance /= test->GetPasses().size();
			summary[i].MemoryUsage /= test->GetPasses().size();
			summary[i].CustomResult /= test->GetPasses().size();
		}
	}

	return unique_ptr<TestResults>(testResults);
}