# CodeCompare
Test harness to compare different implementations of code and generate nice results.

# Create A Test Suite
A Test Suite is a complete environment to execute tests and collate results in a way that can be used to generate visualizations from. Test Suites have 3 major components to them: Parameters, Configurations and Code Tests.

The main process is that you create a new test suite, add your test parameters, customize each test pass as necessary and add the code bindings (See example code below or one of the example projects).

## Parameters
A parameters is a single unit that will be applied in each available code test. Parameters are how the system can make good comparisons of different code samples. Parameters can be as simple or complex as you need them to. The test suite will simple see them as a name, it's up to you how use them.

## Configurations
A configuration is a way to allow you to control how the test information is collected and how the Printer should display it. It has various options for enabling and disabling different parts of the test, as well as adjusting the output from raw values into relative or ranked data.

Each code pass can have a separate configuration as well as each part of the pass's result (Result, Performance and Memory Usage). The Summary also contains it's own configuration.

## Code Tests
A Code Test is a simple objects that is used to bind different code samples into a simple interface that the test suite can invoke. It's basically just a map of Functors for each test pass you want to run on the code sample. There are some cases where not all passes will make sense for all code samples so there isn't a strong binding forcing all Code Tests to implement all passes.

# Invoke a Printer
The second part of implementing a test is figuring out how to print it. You can enable verbose printing to get some info on the command line but the main idea is to use one of the build in printers, or write your own, to convert the data into something presentable, or maybe even upload to elastic search or anything you want. It's basically just processing the results of the test suite.

# Example:
Here's a very basic example showing the major parts of the system:
```c++
#include "Bootstrap.h"
#include "ChartJSPrinter.h"

class ExampleHarness : public TestHarness
{
	unique_ptr<TestSuite const> TestHarness::CreateTest() const override
	{
		TestSuite* suite = new TestSuite("Test Suite");
		suite->AddAutoParameter(1, 10, 100, 10000, 1000000);
		suite->SetPassWeights("pass2", TestWeight(1, 1, 0.1f));

		unique_ptr<CodeTest> test1(new CodeTest("test1"));
		test1->SetPass("pass1", [](Parameter const* param) { return ((IntParameter const*)param)->GetValue() * 50; });
		test1->SetPass("pass2", [](Parameter const* param) { return ((IntParameter const*)param)->GetValue() * 100; });
		suite->AddTest(move(test1));

		unique_ptr<CodeTest> test2(new CodeTest("test2"));
		test2->SetPass("pass1", [](Parameter const* param) { return ((IntParameter const*)param)->GetValue() * 100; });
		test2->SetPass("pass3", [](Parameter const* param) { return ((IntParameter const*)param)->GetValue() * 200; });
		suite->AddTest(move(test2));

		return unique_ptr<TestSuite const>(suite);
	}

	void PrintTest(TestResults const& results) const override
	{
		ChartJSPrinter printer;
		printer.PrintResults(results);
	}
};

void main()
{
	Bootstrap::RunTests(ExampleHarness());
}
```
