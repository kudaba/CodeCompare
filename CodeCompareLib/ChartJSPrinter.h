#pragma once

struct TestResults;

//----------------------------------------------------------------------------------------
// Print the results to a webpage that uses Chart.js as a renderer
// http://www.chartjs.org
//----------------------------------------------------------------------------------------
class ChartJSPrinter
{
public:
	ChartJSPrinter()
		: FullHtml(true)
		, CSSStyle(CSSStyleType::External)
		, CSSFile(nullptr)
		, ScriptStyle(ScriptStyleType::External)
		, ScriptFile(nullptr)
		, HTMLFile("results.html")
		, IncludeD3(true)
		, ChartWidth(500)
		, ChartHeight(200)
		, SpacingX(50)
		, SpacingY(100)
	{

	}

	enum class CSSStyleType
	{
		External,
		Internal,
		Inline
	};

	enum class ScriptStyleType
	{
		External,
		Internal,
	};

	bool FullHtml;

	CSSStyleType CSSStyle;
	const char* CSSFile;

	ScriptStyleType ScriptStyle;
	const char* ScriptFile;

	const char* HTMLFile;

	bool IncludeD3;

	int ChartWidth;
	int ChartHeight;

	int SpacingX;
	int SpacingY;

	void PrintResults(TestResults const& results) const;
};