#include <stdio.h>

#include "packer.h"

int
main(int ac, char *av[])
{
	if (ac != 2) {
		fprintf(stderr, "Usage: woody_woodpacker FILE\n");
		return (1);
	}
	return (packer(av[1]));
}
