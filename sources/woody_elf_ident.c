#include <elf.h>
#include <unistd.h>
#include <fcntl.h>

#include "woody.h"

int woody_elf_ident(const char *filename, unsigned char *class)
{
	unsigned char ident[EI_NIDENT];
	unsigned char elfmag[] = {ELFMAG0, ELFMAG1, ELFMAG2, ELFMAG3};
	int fd;
	ssize_t bytes;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (woody_error(WE_OPEN, filename));
	bytes = read(fd, ident, EI_NIDENT);
	close(fd);
	if (bytes < EI_NIDENT)
		return (woody_error(WE_EI_NIDENT, filename));
	if (ft_memcmp(ident, elfmag, sizeof(elfmag)))
		return (woody_error(WE_EI_ELFMAG, filename));
	if (ident[EI_CLASS] != ELFCLASS32 && ident[EI_CLASS] != ELFCLASS64)
		return (woody_error(WE_EI_CLASS, filename));
	*class = ident[EI_CLASS];
	return (0);
}
