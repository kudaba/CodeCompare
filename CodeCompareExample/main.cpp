#include "Bootstrap.h"
#include "ChartJSPrinter.h"

float testss[] = { -1, 0, 1 };
float* tests = testss;

// Begin Code Segment to Update links at godbolt.org
#include <math.h>

// Extract negative bit manually then do math
int roundbit(float aFloat) {
    int neg = (*(int*)(&aFloat)) >> 31;
    return int(aFloat + 0.5f) * !neg + int(aFloat - 0.5f) * neg;
}

// Use ternary operator
int roundnaive(float aFloat) {
    return aFloat >= 0.0f ? int(aFloat + 0.5f) : int(aFloat - 0.5f);
}

// Test negativity and then do math
int roundopt(float aFloat) {
    bool neg = aFloat < 0;
    return int(aFloat + 0.5f) * !neg + int(aFloat - 0.5f) * neg;
}

template<typename T>
T sign(T val) {
    return T((val > 0) - (val < 0));
}

// Use math to get +1 or -1 and shift half in that direction
int roundsign(float aFloat) {
    return int(aFloat + 0.5f * sign(aFloat));
}

// Use std signbit method then math
int roundsignbit(float aFloat) {
    bool neg = signbit(aFloat);
    return int(aFloat + 0.5f) * !neg + int(aFloat - 0.5f) * neg;
}

// End Code Segment to Update links at godbolt.org

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
        config.Performance.Logarithmic = true;

        //config.Performance.Sort = PassConfig::RelativeValue1Min;
        suite->SetPassConfig("Performance All", config);
        suite->SetPassConfig("Performance Neg", config);
        suite->SetPassConfig("Performance Pos", config);

        CreatePass("roundnaive", roundnaive);
        CreatePass("roundopt", roundopt);
        CreatePass("roundsign", roundsign);
        CreatePass("roundbit", roundbit);
        CreatePass("roundsignbit", roundsignbit);

		return unique_ptr<TestSuite const>(suite);
	}

	void PrintTest(TestResults const& results) const override
	{
        const char* link = "OYLghAFBqd5QCxAYwPYBMCmBRdBLAF1QCcAaPECAKxAEZSBnVAV2OUxAHIBSAJgGY8AO2QAbZlgDU3fgGEAtgEMCCAHQIZ2bgAYAgjt0B6Q5OwAPAsUXICkoZmDK8AN0ySARoUlKhzRaNEAT0kVTCFJdFRvZQ09YVtiFiF0TwIIADNRVGVJRQAxLOUASmkAdgAhA0lqyXi7B2l%2BABFJCAAqCHi2oog%2BADZ8woIiks1NSX5aGUq9GsliTAJWcPiIQezbPnLJbVUAVnSStskwMHtgaV5t1fWcgFod/cPJY/Ppg25Spo%2B9Y0kAVQYbgImGIQkUxGCqAADqDlCQDHVEsxkuCXJgMkNcgUNqMKlUagslmDsVixs1Htp0o08rUhGlbpsro8DiUQHSGTj7izDu89J9vvzfiYACqYBi2c5OZyEYKKZIhBBhCJRJQqRH0%2BZJSLQtKZDak3FlGa6ObuVCoUT1C4yFqMxqyHZ802ExbLDlrLlM7a7VkvE5nBpbD32h6%2B56vBzOgU/XQg%2BTQ0TKTAyWQEQKw8HyNwisZ6EWSBh4YBCCAF5z%2BPEmuZE90iqAVq3jbQlB4QRsOnYjaNfWN/QFuNUIEJRYCLS7lWiSEiSO5T%2BXoQsIPDpWwIfzU4SKnL4BY2PCoIQahLaoslzEGxlVgnVWskm5eic8/1n0tXoo9wX6YUAoGFgiLq%2BqTeIsCAYIqypDseWoougQGEBeORXsaN4eBaVrnI0LTwZyQwfvw1ausSKz0p6ZLMuGRwBphwYPliYZPFRbwER8XycEUpCiFweycKQQhcNovGoFwshbMGTCsOwlyTLxBACexHEANYgHs2icVwAAsvH8ZwgmkMJnC8QwIBqXJunsaQcCwEgaAJngoiguQlC2dC9mgiAwB9PwpDpPZILEMZEDuPJpC2dm9IAPJCEEIW7pg%2B6uMZ5nkPSmBceZHEwgQB5CEldwRfwRksGwHC0Bx6U8XxIUGWYAAcfR3H0GmSMAyDIJIfSqPwrS4IQM58JMpCSLIqB2Q5xDSWVsnySMpDKap6mcFpVXJQZRkmaQZmCbN6W8Np1VcNNGUca4AU5SAGlAA";
        
        string footer;
        footer += "<h3><a href=\"https://godbolt.org/#z:";
        footer += link;
        footer += "\">";
        footer += "Source Code and Assembly generated</a></h3>\n";
        footer += "Courtesy of <a href=\"https://godbolt.org/\">godbolt.org</a><br/>\n";
        footer += "<iframe width=\"100%\" height=\"500px\" src=\"https://godbolt.org/embed-ro#z:";
        footer += link;
        footer += "\"></iframe>\n";

		ChartJSPrinter printer;
        printer.Footer = footer.c_str();
        printer.OneRowPerTest = false;

		printer.PrintResults(results);
        printer.Open();
	}
};

void main()
{
	Bootstrap::RunTests(ExampleHarness());
}