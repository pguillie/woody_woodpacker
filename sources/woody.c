#include <elf.h>

#include "woody.h"

int woody(const char *filename)
{
	struct file_info file;
	unsigned char class;
	int failure;

	if (woody_elf_ident(filename, &class))
		return (1);
	file.name = filename;
	if (woody_map_file(&file))
		return (1);
	if (class == ELFCLASS32)
		failure = woody32(&file);
	else
		failure = woody64(&file);
	if (!failure)
		failure |= woody_write_binary(&file);
	if (woody_unmap_file(&file))
		return (1);
	return (failure);
}
