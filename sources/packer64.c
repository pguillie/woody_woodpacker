#include <elf.h>

#include "packer.h"

static int check_ehdr(struct elf_info *bin)
{
	Elf64_Ehdr *ehdr = (Elf64_Ehdr *)bin->addr;

	if (ehdr->e_type != ET_EXEC && ehdr->e_type != ET_DYN)
		return (error(ERR_E_TYPE, bin->name));
	if (ehdr->e_machine != EM_X86_64)
		return (error(ERR_E_MACHINE, bin->name));
	if (ehdr->e_entry == 0)
		return (error(ERR_E_ENTRY, bin->name));
	if (ehdr->e_phoff == 0)
		return (error(ERR_E_PHOFF, bin->name));
	return (0);
}

static int insert_page(struct elf_info *bin)
{
	Elf64_Ehdr *ehdr = (Elf64_Ehdr *)bin->addr;
	Elf64_Phdr *phdr = (Elf64_Phdr *)((char *)bin->addr + ehdr->e_phoff);
	Elf64_Half phnum = ehdr->e_phnum;

	ehdr->e_shoff = 0;
	ehdr->e_shnum = 0;
	Elf64_Off insertion_off = bin->length;
	while (phnum--) {
		if (phdr->p_offset >= insertion_off)
			phdr->p_offset += 0x1000;
		if (phdr->p_type == PT_LOAD && phdr->p_flags & PF_X) {
			bin->xseg = phdr;
			insertion_off = phdr->p_offset;
		}
		phdr++;
	}
	return (0);
}

int packer64(struct elf_info *bin)
{
	if (check_ehdr(bin))
		return (1);
	if (insert_page(bin))
		return (1);
	return (0);
}
