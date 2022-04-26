#include <hash64.h>

// Compression function for Merkle-Damgard construction.
// This function is generated using the framework provided.
#define mix(h) {					\
	(h) ^= (h) >> 23;		\
	(h) *= 0x2127599bf4325c37ULL;	\
	(h) ^= (h) >> 47; }

uint64_t hash64(uint64_t x, uint64_t seed)
{
	const uint64_t m = 0x880355f21e6d1965ULL;
	uint64_t h = seed ^ (m<<3);

    mix(x);
	h ^= x;
	h *= m;
    mix(h);
	return h;
} 
