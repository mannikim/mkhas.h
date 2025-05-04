// see LICENSE file for copyright and license information

#define mkhas_IMPL
#include "mkhas.h"

int
main(void)
{
	struct HashTable table = mkhas_htable(64);
	struct HashTableInsertResult result =
		mkhas_insert(table, "key", malloc(sizeof(char)));

	if (result.err != mkhas_INSERT_ERR_OK)
	{
		mkhas_free(table);
		return EXIT_FAILURE;
	}

	mkhas_free(table);
	return EXIT_SUCCESS;
}
