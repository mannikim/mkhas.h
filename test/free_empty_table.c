// see LICENSE file for copyright and license information

/*
##DSC attempts to free an empty hash table should do nothing and throw no
##DSC errors
*/

#define mkhas_IMPL
#include "mkhas.h"

int
main(void)
{
	struct HashTable table = mkhas_htable(64);
	mkhas_free(table);
}
