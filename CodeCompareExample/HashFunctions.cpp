#include "HashFunctions.h"

#include "hashmacro3.h"

namespace Hashes
{
	// Original source from
	// (c) Peter Kankowski, 2008. http://smallcode.weblogs.us
	// Hash functions benchmark

	// Bernstein's hash
	unsigned Bernstein(const char *key, size_t len)
	{
		unsigned hash = 5381;
		for (unsigned i = 0; i < len; ++i)
			hash = 33 * hash + key[i];
		return hash ^ (hash >> 16);
	}

	// Kernighan & Ritchie, "The C programming Language", 3rd edition.
	unsigned KernighanRitchie(const char *key, size_t len)
	{
		unsigned hash = 0;
		for (unsigned i = 0; i < len; ++i)
			hash = 31 * hash + key[i];
		return hash ^ (hash >> 16);
	}

	// source?
	unsigned Hash17(const char *key, size_t len)
	{
		unsigned hash = 0;
		for (unsigned i = 0; i < len; ++i) {
			hash = 17 * hash + (key[i] - ' ');
		}
		return hash ^ (hash >> 16);
	}

	// A hash function with multiplier 65599 (from Red Dragon book)
	unsigned Hash65599(const char *key, size_t len)
	{
		unsigned hash = 0;
		for (unsigned i = 0; i < len; ++i)
			hash = 65599 * hash + key[i];
		return hash ^ (hash >> 16);
	}

	// FNV hash, http://isthe.com/chongo/tech/comp/fnv/
	unsigned FNV1a(const char *key, size_t len)
	{
		unsigned hash = 2166136261;
		for (unsigned i = 0; i < len; ++i)
			hash = 16777619 * (hash ^ key[i]);
		return hash ^ (hash >> 16);
	}

	// Peter Weinberger's hash (from Red Dragon book)
	unsigned Weinberger(const char *key, size_t len)
	{
		unsigned h = 0, g;
		for (unsigned i = 0; i < len; ++i) {
			h = (h << 4) + key[i];
			if ((g = (h & 0xF0000000)) != 0)
				h ^= g >> 24 ^ g;
		}
		return h ^ (h >> 16);
	}

	// Paul Hsieh, http://www.azillionmonkeys.com/qed/hash.html
	unsigned PaulHsieh(const char* key, size_t len)
	{
		unsigned hash = (unsigned)len, tmp;
		if (len == 0) return 0;

		size_t rem = len & 3;
		len >>= 2;

		/* Main loop */
		for (; len > 0; len--) {
			hash += *(const unsigned short*)key;
			tmp = (*(const unsigned short*)(key + 2) << 11) ^ hash;
			hash = (hash << 16) ^ tmp;
			key += 2 * sizeof(unsigned short);
			hash += hash >> 11;
		}

		/* Handle end cases */
		switch (rem) {
		case 3:
			hash += *(const unsigned short*)key;
			hash ^= hash << 16;
			hash ^= key[sizeof(unsigned short)] << 18;
			hash += hash >> 11;
			break;
		case 2:
			hash += *(const unsigned short*)key;
			hash ^= hash << 11;
			hash += hash >> 17;
			break;
		case 1:
			hash += *key;
			hash ^= hash << 10;
			hash += hash >> 1;
		}

		/* Force "avalanching" of final 127 bits */
		hash ^= hash << 3;
		hash += hash >> 5;
		hash ^= hash << 4;
		hash += hash >> 17;
		hash ^= hash << 25;
		hash += hash >> 6;

		return hash;
	}

	// Bob Jenkins, http://www.burtleburtle.net/bob/hash/doobs.html
	unsigned OneAtTime(const char* key, size_t len)
	{
		unsigned hash, i;
		for (hash = 0, i = 0; i < len; ++i) {
			hash += key[i];
			hash += (hash << 10);
			hash ^= (hash >> 6);
		}
		hash += (hash << 3);
		hash ^= (hash >> 11);
		hash += (hash << 15);
		return hash;
	}

	// Arash Partow, http://www.partow.net/programming/hashfunctions/
	unsigned ArashPartow(const char* key, size_t len)
	{
		unsigned hash = 0xAAAAAAAA;
		unsigned i = 0;

		for (i = 0; i < len; i++)
			hash ^= ((i & 1) == 0) ? ((hash << 7) ^ (key[i]) ^ (hash >> 3)) :
			(~((hash << 11) ^ (key[i]) ^ (hash >> 5)));

		return hash;
	}




	// My hash functions
	unsigned ConstHash(const char* key, size_t)
	{
		return unsigned(HASH3(key));
	}
}