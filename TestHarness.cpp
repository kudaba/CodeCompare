#include "TestHarness.h"
#include "ChartJSPrinter.h"

namespace TestHarness
{
	unique_ptr<TestSuite const> TestHarness::CreateTest()
	{
		TestSuite* suite = new TestSuite("Test Suite");
		suite->AddTestParameter(1, 10, 100, 10000, 1000000);
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

	void TestHarness::PrintTest(TestResults const& results)
	{
		ChartJSPrinter printer;
		printer.PrintResults(results);
	}
}