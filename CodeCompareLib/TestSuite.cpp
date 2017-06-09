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

        for (auto const& pass : test->GetPasses())
        {
            cout << "    Running Pass: " << pass.first << endl;

            auto& passResults = testResults->Passes[pass.first];

            auto configItr = PassConfigs.find(pass.first);
            passResults.Config = configItr == PassConfigs.end() ? TestConfig() : configItr->second;
            passResults.Min.resize(numParams, INT64_MAX);
            passResults.Max.resize(numParams, INT64_MIN);
            passResults.Reference.resize(numParams, 0);

            auto& results = passResults.Results[test->GetName()];
            results.Results.reserve(numParams);

            bool first = true;

            for (size_t paramIdx = 0; paramIdx < numParams; ++paramIdx)
            {
                auto const& parameter = Parameters[paramIdx];

                if (!first)
                    cout << ", ";

                cout << parameter->ToString();
                first = false;

                TestResult result;

                Memory::Report const presetup = Memory::GetThreadReport();

                if (pass.second.Setup)
                    pass.second.Setup(parameter.get());

                Memory::ResetThreadTracking();
                Memory::Report const membegin = Memory::GetThreadReport();

                __int64 begin = Time::GetCurrentTick();

                result.CustomResult = pass.second.Function(parameter.get());

                __int64 end = Time::GetCurrentTick();

                Memory::Report const memend = Memory::GetThreadReport();

                if (pass.second.Teardown)
                    pass.second.Teardown(parameter.get());

                Memory::Report const postteardown = Memory::GetThreadReport();

                assert(membegin.TotalMemory == memend.TotalMemory);
                assert(membegin.ActiveAllocations == memend.ActiveAllocations);

                assert(presetup.TotalMemory == postteardown.TotalMemory);
                assert(presetup.ActiveAllocations == postteardown.ActiveAllocations);

                result.Performance = end - begin;
                result.MemoryUsage = memend.TrackingMax;

                results.Results.push_back(std::move(result));

                TestResult& min = passResults.Min[paramIdx];
                TestResult& max = passResults.Max[paramIdx];
                TestResult& ref = passResults.Reference[paramIdx];
                ref.CustomResult = parameter->Max();

                for (unsigned i = 0; i < TestConfig::Count; ++i)
                {
                    auto value = result[i];

                    min[i] = value < min[i] ? value : min[i];
                    max[i] = value > max[i] ? value : max[i];
                }
            }

            cout << endl;
        }
    }

    ProcessSummary(testResults);
    SortByRank(testResults);

	return unique_ptr<TestResults>(testResults);
}

void TestSuite::ProcessSummary(TestResults* testResults) const
{
    // convert everything into 0-10,000 range
    const int summaryRange = 10000;

    auto const numParams = Parameters.size();

    // initialize data
    auto& summary = testResults->Summary;
    for (auto const& test : Tests)
    {
        summary.Results[test->GetName()].Results.resize(numParams, 0);
        summary.Min.resize(numParams, 0);
        summary.Max.resize(numParams, 0);
    }
    
    for (auto const& passResults : testResults->Passes)
    {
        auto const& config = passResults.second.Config;

        TestWeight weight;
        auto itr = PassWeights.find(passResults.first);
        if (itr != PassWeights.end())
            weight = itr->second;

        // calculate max ranges
        for (size_t paramIdx = 0; paramIdx < numParams; ++paramIdx)
        {
            auto& testMax = summary.Max[paramIdx];

            auto const& min = passResults.second.Min[paramIdx];
            auto const& max = passResults.second.Max[paramIdx];

            auto range = max - min;

            for (unsigned i = 0; i < TestConfig::Count; ++i)
            {
                if (config[i].Enabled && weight[i] != 0.0f && range[i] > 0)
                {
                    float w = abs(weight[i]);
                    testMax[0] += __int64(summaryRange * w);
                }
            }
        }

        // calculate sums
        for (auto const& pass : passResults.second.Results)
        {
            auto& testSummary = summary.Results[pass.first];
            auto const& results = pass.second.Results;

            for (size_t paramIdx = 0; paramIdx < numParams; ++paramIdx)
            {
                auto& testSum = testSummary.Results[paramIdx];

                auto const& result = results[paramIdx];
                auto const& min = passResults.second.Min[paramIdx];
                auto const& max = passResults.second.Max[paramIdx];

                auto range = max - min;
                auto value = result - min;

                for (unsigned i = 0; i < TestConfig::Count; ++i)
                {
                    if (config[i].Enabled && weight[i] != 0.0f && range[i] > 0)
                    {
                        float w = abs(weight[i]);

                        double percent = double(value[i]) / double(range[i]) * w;
                        if (w < 0)
                            percent = 1 - percent;

                        testSum[0] += __int64(percent * summaryRange);
                    }
                }
            }
        }
    }
}

void TestSuite::SortByRank(TestResults* testResults) const
{
    auto const numParams = Parameters.size();

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
}
