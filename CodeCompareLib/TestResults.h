#pragma once
#include <vector>
#include <map>
#include <string>
#include <assert.h>

using namespace std;

//----------------------------------------------------------------------------------------
// Data collected for each pass
//
// Performance (nanoseconds) and Memory consumtion are standard.
// Memory collection requires that standard new / delete are used and no allocators are used.
// Each pass can also return a custom piece of data to be compared.
//
// Really this is just a glorified vector3... add helper methods as necessary
//----------------------------------------------------------------------------------------
template <typename T, int Default = 0>
struct TestInfo
{
	enum
	{
        Result = 0,
        Perf,
        Memory,
		Count
	};

	TestInfo()
		: CustomResult((T)Default)
		, Performance((T)Default)
		, MemoryUsage((T)Default)
	{
	}

	TestInfo(T init)
		: CustomResult(init)
		, Performance(init)
		, MemoryUsage(init)
	{
	}

	TestInfo(T cust, T perf, T mem)
		: CustomResult(cust)
		, Performance(perf)
		, MemoryUsage(mem)
	{
	}

	TestInfo& operator|=(TestInfo const& other)
	{
		CustomResult = (T)((int)CustomResult | (int)other.CustomResult);
		Performance = (T)((int)Performance | (int)other.Performance);
		MemoryUsage = (T)((int)MemoryUsage | (int)other.MemoryUsage);
		return *this;
	}

	template <typename Func>
	bool Any(Func func) const
	{
		return func(CustomResult) || func(Performance) || func(MemoryUsage);
	}

	T& operator[](unsigned i) { assert(i < Count); return ((T*)this)[i]; }
	T* begin() { return (T*)this; }
	T* end() { return ((T*)this) + Count; }

	T const& operator[](unsigned i) const { assert(i < Count); return ((T const*)this)[i]; }
	T const* begin() const { return (T const*)this; }
	T const* end() const { return ((T const*)this) + Count; }

    TestInfo operator-(TestInfo other) const
    {
        return TestInfo(CustomResult - other.CustomResult, Performance - other.Performance, MemoryUsage - other.MemoryUsage);
    }

	static const char* Name(unsigned i)
	{
		const char* names[] =
		{
			"Result",
			"Performance",
			"Memory",
		};
		return names[i];
	}

	T CustomResult; // user value
	T Performance; // nano seconds
	T MemoryUsage; // bytes
};

struct PassConfig
{
	enum Sorting
	{
		None,
		RelativeValue1Max,		// Show value relative to maximum value
		RelativeValue1Min,		// Show value relative to minimum value
		Ranked,					// Show by rank (lowest = 1, next is 2, etc)
        Percentage,             // Show by percentage parameter max value
	};


	PassConfig(int init)
		: Enabled(!!init)
		, ShowSum(false)
		, ShowAverage(false)
		, Sort(None)
		, Reverse(false)
        , Logarithmic(false)
	{

	}

	bool Enabled;				// enable this pass at all

	// Extra paramters
	bool ShowSum;				// Show a sum of all parameters used
	bool ShowAverage;			// Show the average all parameters used

	Sorting Sort;
	bool Reverse;
    bool Logarithmic;
};

typedef TestInfo<__int64> TestResult;
typedef TestInfo<float, 1> TestWeight;
typedef TestInfo<PassConfig, 1> TestConfig;

//----------------------------------------------------------------------------------------
// Collected results of tests
//
// Data is sorted by each test type (a pass) and contains the results of every code sample tested.
// A summary is also collected. Each passes contribution to the summary can be controlled by setting
// weights for that pass in the test suite.
//----------------------------------------------------------------------------------------
struct TestResults
{
	struct PassResult
	{
		vector<TestResult> Results;
	};

	struct PassResults
	{
		map<string, PassResult> Results;
		TestConfig Config;

		vector<TestResult> Min;
		vector<TestResult> Max;
        vector<TestResult> Reference;
    };

	map<string, PassResults> Passes;

	PassResults Summary;

	vector<string> Parameters;
};
