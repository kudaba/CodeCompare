#include "TestSuite.h"
#include "Time.h"
#include "Memory.h"
#include <algorithm>

unique_ptr<TestResults> TestSuite::RunTests() const
{
	auto const numParams = Parameters.size();

	TestResults* testResults = new TestResults();
	testResults->Summary.Config = SummaryConfig;
	testResults->Parameters.reserve(Parameters.size());
	for (auto const& p : Parameters)
		testResults->Parameters.push_back(p->ToString());

	cout << "Beginning " << GetName() << endl;

	for (auto const& test : Tests)
	{
		cout << "  Running Test: " << test->GetName() << endl;

		auto& summary = testResults->Summary.Results[test->GetName()];
		summary.Results.resize(numParams);

		for (auto const& pass : test->GetPasses())
		{
			cout << "    Running Pass: " << pass.first << endl;

			auto& passResults = testResults->Passes[pass.first];

			auto configItr = PassConfigs.find(pass.first);
			passResults.Config = configItr == PassConfigs.end() ? TestConfig() : configItr->second;
			passResults.Min.resize(numParams, INT64_MAX);
			passResults.Max.resize(numParams, INT64_MIN);

			auto& results = passResults.Results[test->GetName()];
			results.Results.reserve(numParams);

			bool first = true;

			TestWeight weight;
			auto itr = PassWeights.find(pass.first);
			if (itr != PassWeights.end())
				weight = itr->second;

			TestResult sum;

			for (size_t paramIdx = 0; paramIdx < numParams; ++paramIdx)
			{
				auto const& parameter = Parameters[paramIdx];

				if (!first)
					cout << ", ";

				cout << parameter->ToString();
				first = false;

				TestResult result;

				Memory::ResetThreadTracking();
				Memory::Report membegin = Memory::GetThreadReport();


                if (pass.second.Setup)
                    pass.second.Setup(parameter.get());

				__int64 begin = Time::GetCurrentTick();

				result.CustomResult = pass.second.Function(parameter.get());

				__int64 end = Time::GetCurrentTick();

                if (pass.second.Teardown)
                    pass.second.Teardown(parameter.get());

				Memory::Report memend = Memory::GetThreadReport();

				assert(memend.TotalMemory == membegin.TotalMemory);
				assert(memend.ActiveAllocations == membegin.ActiveAllocations);

				result.Performance = end - begin;
				result.MemoryUsage = memend.TrackingMax;

				results.Results.push_back(std::move(result));

				TestResult& min = passResults.Min[paramIdx];
				TestResult& max = passResults.Max[paramIdx];

				for (unsigned i = 0; i < TestConfig::Count; ++i)
				{
					auto value = result[i];
					summary.Results[paramIdx][i] += __int64(value*weight[i]);
					sum[i] += value;
					min[i] = value < min[i] ? value : min[i];
					max[i] = value > max[i] ? value : max[i];
				}
			}

			cout << endl;
		}

		auto passCount = test->GetPasses().size();
		for (auto& result : summary.Results)
		{
			result.Performance /= passCount;
			result.MemoryUsage /= passCount;
			result.CustomResult /= passCount;
		}
	}

	// post process test
	for (auto& passResults : testResults->Passes)
	{
		for (unsigned i = 0; i < TestConfig::Count; ++i)
		{
			if (passResults.second.Config[i].Sort == PassConfig::Ranked)
			{
				// convert from 0 to max ranking values
				for (size_t paramIdx = 0; paramIdx < numParams; ++paramIdx)
				{
					vector<__int64> sortedResults;
					sortedResults.reserve(passResults.second.Results.size());
					
					for (auto& test : passResults.second.Results)
					{
						sortedResults.push_back(test.second.Results[paramIdx][i]);
					}

					sort(sortedResults.begin(), sortedResults.end());

					for (auto& test : passResults.second.Results)
					{
						test.second.Results[paramIdx][i] = (lower_bound(sortedResults.begin(), sortedResults.end(), test.second.Results[paramIdx][i]) - sortedResults.begin()) + 1;
					}
				}
			}
		}
	}

	return unique_ptr<TestResults>(testResults);
}