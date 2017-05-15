#pragma once

//----------------------------------------------------------------------------------------
// Basically a string join but with a custom printer instead of allocating a new string object
//
// Usages:
//		PrintList(vector<int>({1,2,3}), cout);
//		PrintList(vector<int>({1,2,3}), cout, [&](auto i) { cout << i; });
//		PrintList(vector.begin(), vector.end(), cout, [&](auto i) { cout << i; });
//----------------------------------------------------------------------------------------
template<typename List, typename Printer>
void PrintList(List const& list, Printer& printer, const char* seperator = ", ")
{
	bool first = true;
	for (auto const& param : list)
	{
		if (!first)
			printer << seperator;

		printer << param;
		first = false;
	}
}
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
//----------------------------------------------------------------------------------------
template<typename Iterator, typename Printer>
void PrintList(Iterator begin, Iterator end, Printer& printer, const char* seperator = ", ")
{
	bool first = true;
	for (Iterator itr = begin; itr != end; ++itr)
	{
		if (!first)
			printer << seperator;

		printer << *itr;
		first = false;
	}
}
//----------------------------------------------------------------------------------------
template<typename Iterator, typename Printer, typename Functor>
void PrintList(Iterator begin, Iterator end, Printer& printer, Functor func, const char* seperator = ", ")
{
	bool first = true;
	for (Iterator itr = begin; itr != end; ++itr)
	{
		if (!first)
			printer << seperator;

		func(*itr);
		first = false;
	}
}
