// see LICENSE file for copyright and license information

#define mkhas_IMPL
#include "mkhas.h"

int
main(void)
{
	if (mkhas_h64("key") != mkhas_h64("key"))
	{
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
