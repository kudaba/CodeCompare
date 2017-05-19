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

int rountopt(float aFloat)
{
    bool pos = aFloat >= 0.0f;
    return int(aFloat + 0.5f) * pos + int(aFloat - 0.5f) * !pos;
}

int roundsign(float aFloat)
{
    return int(aFloat + 0.5f * sign(aFloat));
}

extern float* tests;

class ExampleHarness : public TestHarness
{
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

        float* data = tests;

        {
            unique_ptr<CodeTest> test(new CodeTest("roundnaive"));
            test->SetPass("Performance All", [data](Parameter const* param)
            {
                int result = 0;
                for (int i = 0, e = ((IntParameter const*)param)->GetValue(); i < e; ++i)
                {
                    result += roundnaive(data[0] + 0.001f);
                    result += roundnaive(data[1] + 0.001f);
                    result += roundnaive(data[2] + 0.001f);
                }
                return result;
            });
            test->SetPass("Performance Neg", [data](Parameter const* param)
            {
                int result = 0;
                for (int i = 0, e = ((IntParameter const*)param)->GetValue(); i < e; ++i)
                {
                    result += roundnaive(data[0] - 1);
                    result += roundnaive(data[0] - 1);
                    result += roundnaive(data[0] - 1);
                }
                return result;
            });
            test->SetPass("Performance Pos", [data](Parameter const* param)
            {
                int result = 0;
                for (int i = 0, e = ((IntParameter const*)param)->GetValue(); i < e; ++i)
                {
                    result += roundnaive(data[2] + 1);
                    result += roundnaive(data[2] + 1);
                    result += roundnaive(data[2] + 1);
                }
                return result;
            });
            suite->AddTest(move(test));
        }

        {
            unique_ptr<CodeTest> test(new CodeTest("rountopt"));
            test->SetPass("Performance All", [data](Parameter const* param)
            {
                int result = 0;
                for (int i = 0, e = ((IntParameter const*)param)->GetValue(); i < e; ++i)
                {
                    result += rountopt(data[0] + 0.001f);
                    result += rountopt(data[1] + 0.001f);
                    result += rountopt(data[2] + 0.001f);
                }
                return result;
            });
            test->SetPass("Performance Neg", [data](Parameter const* param)
            {
                int result = 0;
                for (int i = 0, e = ((IntParameter const*)param)->GetValue(); i < e; ++i)
                {
                    result += rountopt(data[0] - 1);
                    result += rountopt(data[0] - 1);
                    result += rountopt(data[0] - 1);
                }
                return result;
            });
            test->SetPass("Performance Pos", [data](Parameter const* param)
            {
                int result = 0;
                for (int i = 0, e = ((IntParameter const*)param)->GetValue(); i < e; ++i)
                {
                    result += rountopt(data[2] + 1);
                    result += rountopt(data[2] + 1);
                    result += rountopt(data[2] + 1);
                }
                return result;
            });
            suite->AddTest(move(test));
        }



        {
            unique_ptr<CodeTest> test(new CodeTest("roundsign"));
            test->SetPass("Performance All", [data](Parameter const* param)
            {
                int result = 0;
                for (int i = 0, e = ((IntParameter const*)param)->GetValue(); i < e; ++i)
                {
                    result += roundsign(data[0] + 0.001f);
                    result += roundsign(data[1] + 0.001f);
                    result += roundsign(data[2] + 0.001f);
                }
                return result;
            });
            test->SetPass("Performance Neg", [data](Parameter const* param)
            {
                int result = 0;
                for (int i = 0, e = ((IntParameter const*)param)->GetValue(); i < e; ++i)
                {
                    result += roundsign(data[0] - 1);
                    result += roundsign(data[0] - 1);
                    result += roundsign(data[0] - 1);
                }
                return result;
            });
            test->SetPass("Performance Pos", [data](Parameter const* param)
            {
                int result = 0;
                for (int i = 0, e = ((IntParameter const*)param)->GetValue(); i < e; ++i)
                {
                    result += roundsign(data[2] + 1);
                    result += roundsign(data[2] + 1);
                    result += roundsign(data[2] + 1);
                }
                return result;
            });
            suite->AddTest(move(test));
        }

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