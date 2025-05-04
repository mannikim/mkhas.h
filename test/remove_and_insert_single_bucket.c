// see LICENSE file for copyright and license information

#define mkhas_IMPL
#include "mkhas.h"

int
main(void)
{
	struct HashTable ht = mkhas_htable(1);

	int data = 123;

	struct HashTableInsertResult result;

	result = mkhas_insert(ht, "remove", &data);
	if (result.err != mkhas_INSERT_ERR_OK)
	{
		mkhas_clear(ht);
		return EXIT_FAILURE;
	}

	mkhas_remove(ht, result.data);

	result = mkhas_insert(ht, "remove", &data);
	if (result.err != mkhas_INSERT_ERR_OK)
	{
		mkhas_clear(ht);
		return EXIT_FAILURE;
	}

	mkhas_clear(ht);

	return EXIT_SUCCESS;
}
