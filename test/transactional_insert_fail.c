// see LICENSE file for copyright and license information

/*
##DSC mkhas_insert should never alter the state of a hash table when it fails
*/

#include <stddef.h>
#include <stdlib.h>
#define mkhas_IMPL
#include "mkhas.h"

int
main(void)
{
	struct HashTable ht = mkhas_htable(64);
	struct HashTable ht2 = mkhas_htable(64);

	int data = 123;
	int data2 = 321;

	struct HashTableInsertResult result;
	result = mkhas_insert(ht, "inserting", &data);
	if (result.err != mkhas_INSERT_ERR_OK)
	{
		mkhas_clear(ht);
		return EXIT_FAILURE;
	}

	for (size_t i = 0; i < ht.size; i++)
	{
		ht2.buckets[i] = ht.buckets[i];
	}

	result = mkhas_insert(ht, "inserting", &data2);
	if (result.err != mkhas_INSERT_ERR_HAS_KEY)
	{
		return EXIT_FAILURE;
	}

	for (size_t i = 0; i < ht.size; i++)
	{
		if (ht.buckets[i] != ht2.buckets[i])
		{
			return EXIT_FAILURE;
		}

		if (ht.buckets[i] != NULL && ht.buckets[i]->data != &data)
		{
			return EXIT_FAILURE;
		}
	}

	mkhas_clear(ht);
	return EXIT_SUCCESS;
}
