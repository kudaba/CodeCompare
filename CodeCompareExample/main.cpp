#include "Bootstrap.h"
#include "ChartJSPrinter.h"

float testss[] = { -1, 0, 1 };
float* tests = testss;

template<typename T>
T sign(T val)
{
    return T((val > 0) - (val < 0));
}

// Type your code here, or load an example.
int roundnaive(float aFloat) {
    return aFloat >= 0.0f ? int(aFloat + 0.5f) : int(aFloat - 0.5f);
}

int roundsignbit(float aFloat)
{
    bool neg = signbit(aFloat);
    return int(aFloat + 0.5f) * !neg + int(aFloat - 0.5f) * neg;
}

int roundopt(float aFloat)
{
    bool neg = aFloat < 0;
    return int(aFloat + 0.5f) * !neg + int(aFloat - 0.5f) * neg;
}

int roundsign(float aFloat)
{
    return int(aFloat + 0.5f * sign(aFloat));
}

extern float* tests;

class ExampleHarness : public TestHarness
{
#define CreateFunctor(f, mod) { \
    int result = 0; \
    for (int i = 0, e = ((IntParameter const*)param)->GetValue(); i < e; ++i) { \
        result += f(data[0] + mod); \
        result += f(data[1] + mod); \
        result += f(data[2] + mod); \
    } \
    return result; \
}

    #define CreatePass(name, f) { \
        float* data = tests; \
        CodeTest* test = new CodeTest(name); \
        test->SetPass("Performance All", [data](Parameter const* param) CreateFunctor(f, 0.001f)); \
        test->SetPass("Performance Neg", [data](Parameter const* param) CreateFunctor(f, -1)); \
        test->SetPass("Performance Pos", [data](Parameter const* param) CreateFunctor(f, 1)); \
        suite->AddTest(unique_ptr<CodeTest const>(test)); \
    }
    
    unique_ptr<TestSuite const> TestHarness::CreateTest() const override
	{
		TestSuite* suite = new TestSuite("Test Suite");
		suite->AddAutoParameter(1000000, 10000000, 50000000, 100000000);

        TestConfig config(0);
        suite->SetSummaryConfig(config);

        config.Performance.Enabled = true;
        //config.Performance.Sort = PassConfig::RelativeValue1Min;
        suite->SetPassConfig("Performance All", config);
        suite->SetPassConfig("Performance Neg", config);
        suite->SetPassConfig("Performance Pos", config);

        CreatePass("roundnaive", roundnaive);
        CreatePass("roundopt", roundopt);
        CreatePass("roundsign", roundsign);
        CreatePass("roundsignbit", roundsignbit);

		return unique_ptr<TestSuite const>(suite);
	}

	void PrintTest(TestResults const& results) const override
	{
		ChartJSPrinter printer;
		printer.PrintResults(results);
        printer.Open();
	}
};

void main()
{
	Bootstrap::RunTests(ExampleHarness());
}