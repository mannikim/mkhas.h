// see LICENSE file for copyright and license information

#include <stdlib.h>

#define mkhas_IMPL
#include "mkhas.h"

#define BUCKET_COUNT 16

int
main(void)
{
	// creates a hash table. buckets are on the stack so you don't need to
	// manually free them
	// also, this is a macro
	struct HashTable ht = mkhas_htable(BUCKET_COUNT);

	int values[] = {143, 431, 314};

	// inserts items on the table
	// ideally you should check the result of them
	mkhas_insert(ht, "apple", &values[0]);
	mkhas_insert(ht, "orange", &values[1]);
	mkhas_insert(ht, "pie", &values[2]);

	struct HashTableGetResult result = mkhas_get(ht, "orange");
	if (result.err != mkhas_GET_ERR_OK)
	{
		fprintf(stderr, "failed to get key [orange]! :(\n");
		return 1;
	}

	// key is an identifier of an item in a hash table
	struct HashTableKey key = result.data;

	printf("key [orange] is: %d\n", *(int *)key.value->data);

	mkhas_remove(ht, key);

	printf("key [orange] was removed\n");

	mkhas_clear(ht);

	return 0;
}
