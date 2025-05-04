// see LICENSE file for copyright and license information

/*
##ERR should be able to try to insert the same key twice, but return an error
##ERR result
*/

#include <stdlib.h>
#define mkhas_IMPL
#include "mkhas.h"

int
main(void)
{
	struct HashTable table = mkhas_htable(64);
	struct HashTableInsertResult result;

	char *data = malloc(sizeof(char));
	if (data == NULL)
	{
		return EXIT_FAILURE;
	}

	result = mkhas_insert(table, "key", data);
	if (result.err != mkhas_INSERT_ERR_OK)
	{
		free(data);
		mkhas_free(table);
		return EXIT_FAILURE;
	}

	char *data2 = malloc(sizeof(char));
	if (data2 == NULL)
	{
		mkhas_free(table);
		return EXIT_FAILURE;
	}

	result = mkhas_insert(table, "key", data2);
	if (result.err != mkhas_INSERT_ERR_HAS_KEY)
	{
		mkhas_clear(table);
		free(data);
		free(data2);
		return EXIT_FAILURE;
	}

	free(data2);
	mkhas_free(table);
	return EXIT_SUCCESS;
}
