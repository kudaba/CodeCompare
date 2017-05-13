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

	CodeTest(const char* name) : NamedObject(name)	{}

	void SetPass(const char* name, PassCallback callback)
	{
		Passes[name] = callback;
	}

	PassCallback const& GetPass(const char* name) const { return Passes.at(name); }
	auto const& GetPasses() const { return Passes; }

private:
	map<string, PassCallback> Passes;
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
	TestSuite(const char* name) : NamedObject(name) {}

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
	void AddTestParameter(ParamType param)
	{
		Parameters.push_back(MakeParameter(param));
	}

	template <typename ParamType, typename... ParamTypes>
	void AddTestParameter(ParamType param, ParamTypes... params)
	{
		Parameters.push_back(MakeParameter(param));
		AddTestParameter(params...);
	}

	void AddTest(unique_ptr<CodeTest const> test)
	{
		Tests.push_back(move(test));
	}

	void SetPassWeights(const char* pass, TestWeight weight)
	{
		PassWeights[pass] = weight;
	}

	unique_ptr<TestResults> RunTests() const;

private:
	vector<unique_ptr<CodeTest const>> Tests;
	vector<unique_ptr<Parameter const>> Parameters;
	map<string, TestWeight> PassWeights;
};
