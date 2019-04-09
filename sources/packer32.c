#include "packer.h"

int packer32(struct elf_info *bin __attribute__ ((unused)))
{
	return (error(ERR_EI_CLASS32, 0));
}
