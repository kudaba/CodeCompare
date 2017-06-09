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
		file << "div.chart {" << "\n\t";
		PrintChartStyle(file, "\n\t");
		file << endl << "}" << endl;
	}

	void PrintChartStyle(fstream& file, char const* newLine) const
	{
        file << "width: " << ChartWidth << ";" << newLine;
		file << "height: " << ChartHeight << ";" << newLine;
		file << "margin-right: " << SpacingX << ";" << newLine;
		file << "margin-bottom: " << SpacingY << ";";
	}

	void PrintScript(fstream& file) const
	{
		fstream scriptSourceFile("ChartJSWrapper.js", fstream::in);
		scriptSourceFile.seekg(0, scriptSourceFile.end);
		auto length = scriptSourceFile.tellg();
		scriptSourceFile.seekg(0, scriptSourceFile.beg);

		char* data = new char[(size_t)length.seekpos()];
		scriptSourceFile.read(data, length);
		file.write(data, scriptSourceFile.gcount());
		file << endl;

		delete[] data;
	}

	void PrintCharts(fstream& file, TestResults const& results) const
	{
        if (Header)
        {
            file.write(Header, strlen(Header));
            file << endl;
        }

		PrintChartDivs(file, results);
		PrintChartScripts(file, results);

        if (Footer)
        {
            file.write(Footer, strlen(Footer));
            file << endl;
        }
    }

	void PrintChartDivs(fstream& file, TestResults const& results) const
	{
        ItemCount = 0;

        file << "\t<table>" << endl;
        file << "\t\t<tr>" << endl;

        PrintChartDivs(file, results.Summary, "Summary");

		for (auto const& pass : results.Passes)
		{
			PrintChartDivs(file, pass.second, pass.first.c_str());
		}

        file << "\t\t</tr>" << endl;
        file << "\t</table>" << endl;
    }

    void NextRow(fstream& file) const
    {
        file << "\t\t</tr>" << endl;
        file << "\t\t<tr>" << endl;
    }

	void PrintChartDivs(fstream& file, TestResults::PassResults const& pass, const char* title) const
	{
		if (!pass.Config.Any([](auto const& e) {return e.Enabled; }))
			return;

        if (OneRowPerTest && ItemCount > 0)
            NextRow(file);

		for (int i = 0; i < TestConfig::Count; ++i)
		{
			if (pass.Config[i].Enabled)
				PrintChartDiv(file, title, TestConfig::Name(i));
		}
	}

	void PrintChartDiv(fstream& file, const char* title, const char* chart) const
	{
        if (!OneRowPerTest && (ItemCount % ItemsPerRow) == 0)
            NextRow(file);

        file << "\t\t\t<td>" << endl;

		if (CSSStyle == CSSStyleType::Inline)
		{
			file << "\t\t\t<div style=\"";
			PrintChartStyle(file, "");
			file << "\">" << endl;
		}
		else
		{
			file << "\t\t\t<div class=\"chart\">" << endl;
		}

		file << "\t\t\t\t<canvas id=\"" << title << chart << "\" width=\"100\" height=\"100\"></canvas>" << endl;
        file << "\t\t\t</div></td>" << endl;

        ++ItemCount;
    }


	void PrintChartScripts(fstream& file, TestResults const& results) const
	{
		file << "<script>" << endl;

		file << "parameters = [";
		PrintList(results.Parameters, file, [&](auto const& e) { file << "\"" << e.c_str() << "\""; });
		file << "];" << endl;

		PrintChartScripts(file, results.Summary, "Summary");

		for (auto const& pass : results.Passes)
		{
			PrintChartScripts(file, pass.second, pass.first.c_str());
		}

		file << "</script>" << endl;
	}

	void PrintChartScripts(fstream& file, TestResults::PassResults const& pass, const char* title) const
	{
		if (!pass.Config.Any([](auto const& e) {return e.Enabled; }))
			return;

		for (unsigned i = 0; i < TestConfig::Count; ++i)
		{
			if (pass.Config[i].Enabled)
				PrintChartScript(file, pass, title, i);
		}
	}

	static string ColorString(string const& text)
	{
		size_t value = hash<string>()(text);

		int value32 = (value ^ (value >> sizeof(value)*4)) & 0xffffffff;

		int r = (value32 >> 0) & 0xff;
		int g = (value32 >> 16) & 0xff;
		int b = (value32 >> 8) & 0xff;

		char out[32];
		sprintf_s(out, "rgba(%d,%d,%d,1)", r, g, b);
		return out;
	}

	void PrintChartScript(fstream& file, TestResults::PassResults const& pass, const char* title, unsigned index) const
	{
		const char* chartName = TestConfig::Name(index);

		const char* parameters = "parameters";
		if (pass.Config[index].ShowSum || pass.Config[index].ShowAverage)
		{
			parameters = "params";
			file << "var params = parameters.slice(0);" << endl;
			if (pass.Config[index].ShowSum)
			{
				file << "params.push(\"Totals\");" << endl;
			}
			if (pass.Config[index].ShowAverage)
			{
				file << "params.push(\"Average\");" << endl;
			}
		}

		file << "CreateChart({" << endl;

		file << "\ttest: \"" << title << ": " << chartName << "\"," << endl;
		file << "\tid: \"" << title << chartName << "\"," << endl;
		file << "\tparameters: " << parameters << "," << endl;

        // options
        if (pass.Config[index].Logarithmic && pass.Config[index].Sort == PassConfig::None)
        {
            file << "\ttype: 'logarithmic'," << endl;
        }

        if (pass.Config[index].Sort == PassConfig::Percentage)
        {
            file << "\tmax: 1.0," << endl;
        }

        // data
		file << "\tpasses: [" << endl;

		function<void(vector<TestResult> const&)> resultPrinter = [&](auto const& results) {
			PrintList(results, file, [&](auto const& r) { file << to_string(r[index]).c_str(); }); };

		const char* nl = ",\n";

		PrintList(pass.Results, file, [&](auto const& itr) {
			PrintPassResults(file, itr.first.c_str(), itr.second.Results, pass, index);
		}, nl);

		file << endl << "\t]" << endl;

        file << "});" << endl;
	}

	void PrintPassResults(fstream& file, const char* name, vector<TestResult> const& results, TestResults::PassResults const& pass, unsigned index) const
	{
		file << "\t\t{" << endl;
		file << "\t\t\tname: \"" << name << "\"," << endl;
		file << "\t\t\tresults: [";

		PrintResults(file, results, pass, index);

		file << "]," << endl;
		file << "\t\t\tcolor: \"" << ColorString(name).c_str() << "\"" << endl;
		file << "\t\t}";
	}

	void PrintResults(fstream& file, vector<TestResult> const& results, TestResults::PassResults const& pass, unsigned index) const
	{
		auto sort = pass.Config[index].Sort;
		switch (sort)
		{
		case PassConfig::RelativeValue1Min:
		case PassConfig::RelativeValue1Max:
        case PassConfig::Percentage:
        {
			int paramIdx = 0;
			float sum = 0;
			PrintList(results, file, [&](auto const& r)
			{
				__int64 div =
                    sort == PassConfig::RelativeValue1Min ? pass.Min[paramIdx][index] :
                    sort == PassConfig::RelativeValue1Max ? pass.Max[paramIdx][index] :
                    pass.Reference[paramIdx][index];

				float val = div == 0 ? 1.0f : float(double(r[index]) / double(div));
				sum += val;
				file << val;

				++paramIdx;
			});

			if (pass.Config[index].ShowSum)
				file << ", " << sum;
			if (pass.Config[index].ShowAverage)
				file << ", " << sum / float(results.size());
		}
		break;
		default:
			__int64 sum = 0;

			PrintList(results, file, [&](auto const& r) { sum += r[index]; file << r[index]; });
			if (pass.Config[index].ShowSum)
				file << ", " << sum;
			if (pass.Config[index].ShowAverage)
				file << ", " << sum / results.size();
			break;
		}
	}
};

void ChartJSPrinter::PrintResults(TestResults const& results) const
{
	auto internal = (ChartJSPrinterInternal const*)this;
	internal->PrintResultsInternal(results);
}

void ChartJSPrinter::Open() const
{
    system(HTMLFile);
}
