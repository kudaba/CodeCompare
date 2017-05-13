#pragma once

#include "TestSuite.h"

//----------------------------------------------------------------------------------------
// As a user you simply need to fill out these methods. Create the test suite by
// filling the test parameters and test bindings.
//
// Once the test is finished running it will be given back to print out however you like.
//----------------------------------------------------------------------------------------
namespace TestHarness
{
	unique_ptr<TestSuite const> CreateTest();
	void PrintTest(TestResults const& results);
};