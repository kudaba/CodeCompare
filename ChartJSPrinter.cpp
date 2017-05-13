#include "ChartJSPrinter.h"

#include "TestResults.h"
#include "PrintHelpers.h"

#include <fstream>
#include <functional>

class ChartJSPrinterInternal : public ChartJSPrinter
{
public:
	void PrintResultsInternal(TestResults const& results) const
	{
		fstream file(HTMLFile, fstream::out);

		if (FullHtml)
		{
			PrintFullHtml(file, results);
		}
		else
		{
			if (IncludeD3)
				PrintIncludes(file);

			PrintCharts(file, results);
		}
	}
private:
	void PrintFullHtml(fstream& file, TestResults const& results) const
	{
		file << "<!DOCTYPE html>" << endl;
		file << "<html>" << endl;
		file << "<head>" << endl;
		file << "<meta charset=\"utf - 8\">" << endl;
		PrintIncludes(file);
		file << "</head>" << endl;
		file << "<body>" << endl;
		PrintCharts(file, results);
		file << "</body>" << endl;
		file << "</html>" << endl;
	}

	void PrintIncludes(fstream& file) const
	{
		if (CSSStyle == CSSStyleType::External)
		{
			string cssfileName = CSSFile ? CSSFile : HTMLFile;
			if (!CSSFile)
				cssfileName += ".css";

			fstream cssFile(cssfileName, fstream::out);
			PrintCSS(cssFile);

			file << "<link rel=\"stylesheet\" type=\"text/css\" href=\"" << cssfileName.c_str() << "\"></script>" << endl;
		}
		else if (CSSStyle == CSSStyleType::Internal)
		{
			file << "<style>" << endl;
			PrintCSS(file);
			file << "<style>" << endl;
		}

		file << "<script src=\"https://cdnjs.cloudflare.com/ajax/libs/Chart.js/2.5.0/Chart.min.js\"></script>" << endl;

		if (ScriptStyle == ScriptStyleType::External)
		{
			string scriptFileName = ScriptFile ? ScriptFile : HTMLFile;
			if (!ScriptFile)
				scriptFileName += ".js";

			fstream scriptFile(scriptFileName, fstream::out);
			PrintScript(scriptFile);

			file << "<script src=\"" << scriptFileName.c_str() << "\"></script>" << endl;
		}
		else
		{
			file << "<script>" << endl;
			PrintScript(file);
			file << "</script>" << endl;
		}
	}

	void PrintCSS(fstream& file) const
	{
		file << "div.chart {" << endl;
		PrintChartStyle(file, "\n\t");
		file << "}" << endl;
	}

	void PrintChartStyle(fstream& file, char const* newLine) const
	{
		file << "float: left;" << newLine;
		file << "width: " << ChartWidth << "px;" << newLine;
		file << "height: " << ChartHeight << "px;" << newLine;
		file << "margin-right: " << SpacingX << "px;" << newLine;
		file << "margin-bottom: " << SpacingY << "px;" << newLine;
	}

	void PrintScript(fstream& file) const
	{
		fstream scriptSourceFile("ChartJSWrapper.js", fstream::in);
		scriptSourceFile.seekg(0, scriptSourceFile.end);
		auto length = scriptSourceFile.tellg();
		scriptSourceFile.seekg(0, scriptSourceFile.beg);

		char* data = new char[length.seekpos()];
		scriptSourceFile.read(data, length);
		file.write(data, scriptSourceFile.gcount());
		file << endl;

		delete[] data;
	}

	void PrintCharts(fstream& file, TestResults const& results) const
	{
		PrintChartDivs(file, results);
		PrintChartScripts(file, results);
	}

	void PrintChartDivs(fstream& file, TestResults const& results) const
	{
		if (PrintSummary)
		{
			PrintChartDivs(file, results.Summary, "Summary");
		}

		for (auto const& pass : results.Passes)
		{
			PrintChartDivs(file, pass.second, pass.first.c_str());
		}
	}

	void PrintChartDivs(fstream& file, TestResults::PassResults const& pass, const char* title) const
	{
		file << "\t<div>" << endl;

		PrintChartDiv(file, pass, title, "Results");
		if (IncludePerformance)
		{
			PrintChartDiv(file, pass, title, "Performance");
		}
		if (IncludeMemory)
		{
			PrintChartDiv(file, pass, title, "Memory");
		}

		file << "\t</div>" << endl;
	}

	void PrintChartDiv(fstream& file, TestResults::PassResults const& pass, const char* title, const char* chart) const
	{
		if (CSSStyle == CSSStyleType::Inline)
		{
			file << "\t\t<div style=\"";
			PrintChartStyle(file, "");
			file << "\">" << endl;
		}
		else
		{
			file << "\t\t<div class=\"chart\">" << endl;
		}

		file << "\t\t\t<canvas id=\"" << title << chart << "\"></canvas>" << endl;
		file << "\t\t</div>" << endl;
	}


	void PrintChartScripts(fstream& file, TestResults const& results) const
	{
		file << "<script>" << endl;

		file << "parameters = [";
		PrintList(results.Parameters, file, [&](auto const& e) { file << e.c_str(); });
		file << "];" << endl;

		if (PrintSummary)
		{
			PrintChartScripts(file, results.Summary, "Summary");
		}

		for (auto const& pass : results.Passes)
		{
			PrintChartScripts(file, pass.second, pass.first.c_str());
		}

		file << "</script>" << endl;
	}

	void PrintChartScripts(fstream& file, TestResults::PassResults const& pass, const char* title) const
	{
		PrintChartScript(file, pass, title, "Results", [](auto p) { return p.CustomResult; });
		if (IncludePerformance)
		{
			PrintChartScript(file, pass, title, "Performance", [](auto p) { return p.Performance; });
		}
		if (IncludeMemory)
		{
			PrintChartScript(file, pass, title, "Memory", [](auto p) { return p.MemoryUsage; });
		}
	}

	static string ColorString(string const& text)
	{
		size_t value = hash<string>()(text);

		int value32 = (value ^ (value >> 32)) & 0xffffffff;

		int r = (value32 >> 0) & 0xff;
		int g = (value32 >> 16) & 0xff;
		int b = (value32 >> 8) & 0xff;

		char out[32];
		sprintf_s(out, "rgba(%d,%d,%d,1)", r, g, b);
		return out;
	}

	void PrintChartScript(fstream& file, TestResults::PassResults const& pass, const char* title, const char* chart, function<__int64(TestResult)> result) const
	{
		file << "CreateChart({" << endl;

		file << "\ttest: \"" << title << ": " << chart << "\"," << endl;
		file << "\tid: \"" << title << chart << "\"," << endl;
		file << "\tparameters: parameters," << endl;
		file << "\tpasses: [" << endl;

		PrintList(pass, file, [&](auto const& itr) {
			file << "\t\t{" << endl;
			file << "\t\t\tname: \"" << itr.first.c_str() << "\"," << endl;
			file << "\t\t\tresults: [";
			PrintList(itr.second, file, [&](auto const& r) { file << to_string(result(r)).c_str(); });
			file << "]," << endl;
			file << "\t\t\tcolor: \"" << ColorString(itr.first.c_str()).c_str() << "\"" << endl;
			file << "\t\t}";
		}, ",\n");

		file << endl << "\t]" << endl;
		file << "});" << endl;
	}
};

void ChartJSPrinter::PrintResults(TestResults const& results) const
{
	auto internal = (ChartJSPrinterInternal const*)this;
	internal->PrintResultsInternal(results);
}