#pragma once

template<typename List, typename Printer, typename Functor>
void PrintJoin(List const& list, Printer& printer, Functor func)
{
	bool first = true;
	for (auto const& param : list)
	{
		if (!first)
			printer << ", ";

		printer << func(param);
		first = false;
	}
}

template<typename List, typename Printer, typename Functor>
void PrintList(List const& list, Printer& printer, Functor func)
{
	bool first = true;
	for (auto const& param : list)
	{
		if (!first)
			printer << ", ";

		func(param);
		first = false;
	}
}