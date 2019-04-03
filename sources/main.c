#include <stdio.h>

#include "woody.h"

int main(int ac, char *av[])
{
	if (ac != 2) {
		fprintf(stderr, "Usage: woody_woodpacker FILE\n");
		return (1);
	}
	return (woody(av[1]));
}
