#include <elf.h>

#include "packer.h"

int packer(const char *filename)
{
	struct elf_info bin;
	unsigned char class;
	int failure;

	if (elf_ident(filename, &class))
		return (1);
	bin.name = filename;
	if (map_file(&bin))
		return (1);
	if (class == ELFCLASS32)
		failure = packer32(&bin);
	else
		failure = packer64(&bin);
	if (!failure)
		failure = write_woody(&bin);
	if (unmap_file(&bin))
		return (1);
	return (failure ? 1 : 0);
}
