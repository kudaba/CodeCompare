#pragma once

#include <iostream>
#include <memory>
#include <set>
#include <functional>

#include "TestResults.h"

//----------------------------------------------------------------------------------------
// just save a little code duplications
//----------------------------------------------------------------------------------------
class NamedObject
{
protected:
	NamedObject(const char* name) : Name(name) {}
public:
	auto const& GetName() const { return Name; }
private:
	string Name;
};

//----------------------------------------------------------------------------------------
// Parameter wrapper, can be anything the test wants
//----------------------------------------------------------------------------------------
class Parameter
{
public:
	virtual ~Parameter() {}

	virtual string ToString() const = 0;

    virtual __int64 Max() const { return 1; }
};

//----------------------------------------------------------------------------------------
// useful for simple types
//----------------------------------------------------------------------------------------
template <typename Type>
class BasicParameter : public Parameter
{
public:
	BasicParameter(Type&& value) : Value(move(value)) {}

	Type const& GetValue() const { return Value; }

	string ToString() const override { return to_string(Value); }
private:
	Type Value;
};

typedef BasicParameter<int> IntParameter;
typedef BasicParameter<long long> Int64Parameter;
typedef BasicParameter<float> FloatParameter;

//----------------------------------------------------------------------------------------
// Wrapper for a single code sample to be tested.
//
// For each type of test to perform (a pass) bind a callback to the code samples version of the pass
// The parameter type is the control data that is sent at each interval to be tested
//----------------------------------------------------------------------------------------
class CodeTest : public NamedObject
{
public:
	typedef function<__int64(Parameter const*)> PassCallback;
    typedef function<void(Parameter const*)> PassSetup;

    struct Pass
    {
        PassSetup Setup;
        PassCallback Function;
        PassSetup Teardown;
    };

	CodeTest(const char* name) : NamedObject(name)	{}
    virtual ~CodeTest() {}

	void SetPass(const char* name, PassCallback callback)
	{
		Passes[name].Function = callback;
	}
    void SetPassSetup(const char* name, PassSetup callback)
    {
        Passes[name].Setup = callback;
    }
    void SetPassTeardown(const char* name, PassSetup callback)
    {
        Passes[name].Teardown = callback;
    }

    Pass const& GetPass(const char* name) const { return Passes.at(name); }
	auto const& GetPasses() const { return Passes; }

private:
	map<string, Pass> Passes;
};

//----------------------------------------------------------------------------------------
// The test suite will take a set of code samples and a parameter set.
// For each pass in the code sample it will run it against each parameter and record the results.
//
// It will summarize all the passes together using optional weighting of each pass.
//----------------------------------------------------------------------------------------
class TestSuite : public NamedObject
{
public:
	TestSuite(const char* name)
        : NamedObject(name)
        , SummaryConfig(PassConfig(1), PassConfig(0), PassConfig(0))
    {
        SummaryConfig[0].Sort = PassConfig::RelativeValue1Max;
        SummaryConfig[0].ShowAverage = true;
    }

	static unique_ptr<Parameter const> MakeParameter(unique_ptr<Parameter const> parameter)
	{
		return parameter;
	}

	template <typename ParamType>
	static unique_ptr<Parameter const> MakeParameter(ParamType param)
	{
		return unique_ptr<Parameter const>(new BasicParameter<ParamType>(move(param)));
	}

	template <typename ParamType>
	void AddAutoParameter(ParamType param)
	{
		Parameters.push_back(MakeParameter(param));
	}

	template <typename ParamType, typename... ParamTypes>
	void AddAutoParameter(ParamType param, ParamTypes... params)
	{
		Parameters.push_back(MakeParameter(param));
		AddAutoParameter(params...);
	}

	void AddTestParameter(unique_ptr<Parameter const> param)
	{
		Parameters.push_back(move(param));
	}

	void AddTest(unique_ptr<CodeTest const> test)
	{
		Tests.push_back(move(test));
	}

	void SetPassWeights(const char* pass, TestWeight weight)
	{
		PassWeights[pass] = weight;
	}

	void SetPassConfig(const char* pass, TestConfig config)
	{
		PassConfigs[pass] = config;
	}

	void SetSummaryConfig(TestConfig config)
	{
		SummaryConfig = config;
	}

	unique_ptr<TestResults> RunTests() const;

private:
    void ProcessSummary(TestResults* testResults) const;
    void SortByRank(TestResults* testResults) const;

	vector<unique_ptr<CodeTest const>> Tests;
	vector<unique_ptr<Parameter const>> Parameters;
	map<string, TestWeight> PassWeights;
	map<string, TestConfig> PassConfigs;
	TestConfig SummaryConfig;
};
