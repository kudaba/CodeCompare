#pragma once

//----------------------------------------------------------------------------------------
// Basically a string join but with a custom printer instead of allocating a new string object
//
// Usage: PrintList(vector<int>({1,2,3}), cout, [&](auto i) { cout << i; });
//----------------------------------------------------------------------------------------
template<typename List, typename Printer, typename Functor>
void PrintList(List const& list, Printer& printer, Functor func, const char* seperator = ", ")
{
	bool first = true;
	for (auto const& param : list)
	{
		if (!first)
			printer << seperator;

		func(param);
		first = false;
	}
}