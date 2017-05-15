#pragma once

namespace Hashes
{
	// Original source from
	// (c) Peter Kankowski, 2008. http://smallcode.weblogs.us
	// Hash functions benchmark

	// Bernstein's hash
	unsigned Bernstein(const char *key, size_t len);

	// Kernighan & Ritchie, "The C programming Language", 3rd edition.
	unsigned KernighanRitchie(const char *key, size_t len);

	// Source?
	unsigned Hash17(const char *key, size_t len);

	// A hash function with multiplier 65599 (from Red Dragon book)
	unsigned Hash65599(const char *key, size_t len);

	// FNV hash, http://isthe.com/chongo/tech/comp/fnv/
	unsigned FNV1a(const char *key, size_t len);

	// Peter Weinberger's hash (from Red Dragon book)
	unsigned Weinberger(const char *key, size_t len);

	// Paul Hsieh, http://www.azillionmonkeys.com/qed/hash.html
	unsigned PaulHsieh(const char* key, size_t len);

	// Bob Jenkins, http://www.burtleburtle.net/bob/hash/doobs.html
	unsigned OneAtTime(const char* key, size_t len);

	// Arash Partow, http://www.partow.net/programming/hashfunctions/
	unsigned ArashPartow(const char* key, size_t len);


	// My hash functions
	unsigned ConstHash(const char* key, size_t);
}