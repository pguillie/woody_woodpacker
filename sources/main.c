#include <stdio.h>

#include "packer.h"
#include "program.h"

char *program;

int
main(int ac, char *av[])
{
	program = av[0];
	if (ac != 2)
		return (error(ERR_USAGE, NULL));
	return (packer(av[1]));
}
