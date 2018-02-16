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
        , Header(nullptr)
        , Footer(nullptr)
        , HTMLFile("results.html")
		, OutputPath(nullptr)
		, IncludeD3(true)
		, ChartWidth("500px")
		, ChartHeight("500px")
		, SpacingX("50px")
		, SpacingY("100px")
        , OneRowPerTest(true)
        , ItemsPerRow(3)
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

    const char* Header;
    const char* Footer;

	const char* HTMLFile;
	const char* OutputPath;

	bool IncludeD3;

    const char* ChartWidth;
    const char* ChartHeight;

    const char* SpacingX;
    const char* SpacingY;

    bool OneRowPerTest;
    int ItemsPerRow; // only applies if OneRowPerTest is false

	void PrintResults(TestResults const& results) const;
    void Open() const;

protected:
    mutable int ItemCount;
};