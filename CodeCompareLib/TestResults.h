#pragma once
#include <vector>
#include <map>
#include <string>

using namespace std;

//----------------------------------------------------------------------------------------
// Data collected for each pass
//
// Performance (nanoseconds) and Memory consumtion are standard.
// Memory collection requires that standard new / delete are used and no allocators are used.
// Each pass can also return a custom piece of data to be compared.
//----------------------------------------------------------------------------------------
template <typename T, int Default = 0>
struct TestInfo
{
	TestInfo()
		: Performance(Default)
		, MemoryUsage(Default)
		, CustomResult(Default)
	{
	}

	TestInfo(T perf, T mem, T cust)
		: Performance(perf)
		, MemoryUsage(mem)
		, CustomResult(cust)
	{
	}

	T Performance; // nano seconds
	T MemoryUsage; // bytes
	T CustomResult; // user value
};

typedef TestInfo<__int64> TestResult;
typedef TestInfo<float, 1> TestWeight;

//----------------------------------------------------------------------------------------
// Collected results of tests
//
// Data is sorted by each test type (a pass) and contains the results of every code sample tested.
// A summary is also collected. Each passes contribution to the summary can be controlled by setting
// weights for that pass in the test suite.
//----------------------------------------------------------------------------------------
struct TestResults
{
	typedef map<string, vector<TestResult>> PassResults;
	map<string, PassResults> Passes;

	PassResults Summary;

	vector<string> Parameters;
};
