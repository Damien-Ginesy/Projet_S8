#include "joaat_hash.h"

uint32_t joaat_hash(void *key, uint32_t len)
{
	uint32_t hash = 0;
	for (uint32_t i = 0; i < len; i++)
	{
		hash += ((uint8_t*)key)[i];
		hash += (hash << 10);
		hash ^= (hash >> 6);
	}
	hash += (hash << 3);
	hash ^= (hash >> 11);
	hash += (hash << 15);
	return hash;
}