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
	if (packer(av[1])) {
		fprintf(stderr, "Exiting...\n");
		return (1);
	}
	printf("'%s' successfully packed! Output binary name: 'woody'\n",
		av[1]);
	return (0);
}
