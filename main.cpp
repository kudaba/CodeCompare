#include "TestSuite.h"
#include "ChartJSPrinter.h"
#include "PrintHelpers.h"

void PrintPass(const char* name, TestResults::PassResults const& pass)
{
	cout << name << endl;

	for (auto const& test : pass)
	{
		cout << test.first << endl;

		cout << "Perf: ";
		PrintJoin(test.second, cout, [](auto& item) { return item.Performance; });
		cout << endl;

		cout << "Mem: ";
		PrintJoin(test.second, cout, [](auto& item) { return item.MemoryUsage; });
		cout << endl;

		cout << "Custom: ";
		PrintJoin(test.second, cout, [](auto& item) { return item.CustomResult; });
		cout << endl;
	}
	cout << endl;
}

void main()
{
	TestSuite suite("Test Suite");
	suite.AddTestParameter(1, 10, 100, 10000, 1000000);
	suite.SetPassWeights("pass2", TestWeight(1,1,0.1f));

	unique_ptr<CodeTest> test1(new CodeTest("test1"));
	test1->SetPass("pass1", [](Parameter const* param) { return ((IntParameter const*)param)->GetValue() * 50; });
	test1->SetPass("pass2", [](Parameter const* param) { return ((IntParameter const*)param)->GetValue() * 100; });
	suite.AddTest(move(test1));

	unique_ptr<CodeTest> test2(new CodeTest("test2"));
	test2->SetPass("pass1", [](Parameter const* param) { return ((IntParameter const*)param)->GetValue() * 100; });
	test2->SetPass("pass3", [](Parameter const* param) { return ((IntParameter const*)param)->GetValue() * 200; });
	suite.AddTest(move(test2));

	auto results = suite.RunTests();

	cout << endl << endl;

	for (auto const& pass : results->Passes)
	{
		PrintPass(pass.first.c_str(), pass.second);
	}

	PrintPass("Summary", results->Summary);

	ChartJSPrinter printer;
	printer.PrintResults(*results);


	printer.CSSStyle = ChartJSPrinter::CSSStyleType::Inline;
	printer.ScriptStyle = ChartJSPrinter::ScriptStyleType::Internal;
	printer.HTMLFile = "results2.html";
	printer.PrintResults(*results);
}