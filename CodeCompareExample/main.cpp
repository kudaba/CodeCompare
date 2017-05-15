#include "Bootstrap.h"
#include "ChartJSPrinter.h"

#include "HashFunctions.h"
#include <Windows.h>
#include <fstream>

namespace Hashes
{
	typedef unsigned(*HashFunction)(const char*, size_t);
	static const HashFunction Functions[] = { Bernstein, KernighanRitchie, Hash17,
		Hash65599, FNV1a, Weinberger, PaulHsieh, OneAtTime, ArashPartow,
		ConstHash };
	static const char* Names[] = { "Bernstein", "Kernighan&Ritchie", "x17",
		"x65599", "FNV-1a", "Weinberger", "Paul Hsieh", "One At Time", "Arash Partow",
		"Const Hash (mine)" };

	const unsigned Count = sizeof(Functions) / sizeof(Functions[0]);
	const unsigned MaxLength = 64; // Const Hash caps at this so to keep the comparison fair ish
}

class HashParameter : public Parameter, public NamedObject
{
public:
	HashParameter(const char* fileName) : NamedObject(fileName)
	{
		fstream file(fileName);

		char line[1024];
		while (!file.eof())
		{
			file.getline(line, 1024);

			int const max = Hashes::MaxLength;
			int len = (int)strlen(line);
			int offset = 0;
			while (len > 0)
			{
				Data.emplace_back(line + offset, len > max ? max : len);
				len -= max;
				offset += max;
			}
		}
	}

	static unsigned NextPowerOfTwo(unsigned x) {
		// Henry Warren, "Hacker's Delight", ch. 3.2
		x--;
		x |= (x >> 1);
		x |= (x >> 2);
		x |= (x >> 4);
		x |= (x >> 8);
		x |= (x >> 16);
		return x + 1;
	}

	__int64 RunTest(Hashes::HashFunction function, bool trueCollisions) const
	{
		struct Element
		{
			Element() : Key(nullptr), Hash(0) {}

			const char* Key;
			unsigned Hash;
		};

		unsigned size = NextPowerOfTwo((unsigned)Data.size()) * 2;
		unsigned sizemask = size - 1;
		vector<Element> table;
		table.resize(size);

		__int64 collision = 0;
		__int64 truecollision = 0;

		for (auto const& line : Data)
		{
			unsigned hash = function(line.c_str(), line.size());
			unsigned index = hash & sizemask;

			while (table[index].Key != nullptr)
			{
				index = (index + 1) & sizemask;
				++collision;
				if (table[index].Hash == hash)
					++truecollision;
			}

			table[index].Key = line.c_str();
			table[index].Hash = hash;
		}

		return trueCollisions ? truecollision : collision;
	}

	__int64 Collisions(Hashes::HashFunction function) const
	{
		return RunTest(function, false);
	}

	__int64 TrueCollisions(Hashes::HashFunction function) const
	{
		return RunTest(function, true);
	}

	__int64 Performance(Hashes::HashFunction function) const
	{
		for (auto const& line : Data)
			function(line.c_str(), line.size());
		return 0;
	}

	string ToString() const override { return GetName(); }

private:
	vector<string> Data;
};

class ExampleHarness : public TestHarness
{
	static void LoadParameter(TestSuite* suite)
	{
		WIN32_FIND_DATA fileData;
		HANDLE hFiles;

		hFiles = FindFirstFile("*.txt", &fileData);

		if (INVALID_HANDLE_VALUE == hFiles)
		{
			printf("No txt files found.\n");
			return ;
		}

		do
		{
			if (fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
			}
			else
			{
				if (fileData.nFileSizeHigh)
				{
					cout << fileData.cFileName << " too large to proccess." << endl;
				}
				else
				{
					suite->AddTestParameter(unique_ptr<Parameter const>(new HashParameter(fileData.cFileName)));
				}
			}
		} while (FindNextFile(hFiles, &fileData) != 0);
	}

	unique_ptr<TestSuite const> TestHarness::CreateTest() const override
	{
		TestSuite* suite = new TestSuite("Hash Function Test");
		LoadParameter(suite);

		TestConfig config(0);
		config.CustomResult.Enabled = true;
		config.CustomResult.ShowAverage = true;
		config.CustomResult.Sort = PassConfig::Ranked;

		TestConfig perfConfig(0);
		perfConfig.Performance.Enabled = true;
		perfConfig.Performance.ShowAverage = true;
		perfConfig.Performance.Sort = PassConfig::RelativeValue1Max;

		suite->SetPassConfig("Collisions", config);
		suite->SetPassConfig("True Collisions", config);
		suite->SetPassConfig("Performance", perfConfig);

		for (int i = 0; i < Hashes::Count; ++i)
		{
			CodeTest* test = new CodeTest(Hashes::Names[i]);
			test->SetPass("Collisions", [i](Parameter const* param) { return ((HashParameter const*)param)->Collisions(Hashes::Functions[i]); });
			test->SetPass("True Collisions", [i](Parameter const* param) { return ((HashParameter const*)param)->TrueCollisions(Hashes::Functions[i]); });
			test->SetPass("Performance", [i](Parameter const* param) { return ((HashParameter const*)param)->Performance(Hashes::Functions[i]); });

			suite->AddTest(unique_ptr<CodeTest const>(test));
		}

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