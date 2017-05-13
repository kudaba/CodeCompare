#pragma once

#include "TestSuite.h"

//----------------------------------------------------------------------------------------
// As a user you simply need to fill out these methods. Create the test suite by
// filling the test parameters and test bindings.
//
// Once the test is finished running it will be given back to print out however you like.
//----------------------------------------------------------------------------------------
class TestHarness
{
public:
	TestHarness() : Verbose(false) {}

	virtual unique_ptr<TestSuite const> CreateTest() const = 0;
	virtual void PrintTest(TestResults const& results) const = 0;

	bool Verbose;
};