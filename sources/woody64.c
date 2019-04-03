#include <elf.h>

#include "woody.h"

static int woody64_check_ehdr(struct file_info *file)
{
	Elf64_Ehdr *ehdr = (Elf64_Ehdr *)file->addr;

	if (ehdr->e_type != ET_EXEC && ehdr->e_type != ET_DYN)
		return (woody_error(WE_E_TYPE, file->name));
	if (ehdr->e_machine != EM_X86_64)
		return (woody_error(WE_E_MACHINE, file->name));
	if (ehdr->e_entry == 0)
		return (woody_error(WE_E_ENTRY, file->name));
	if (ehdr->e_phoff == 0)
		return (woody_error(WE_E_PHOFF, file->name));
	return (0);
}
#include <stdio.h>
static int woody64_insert_page(struct file_info *file)
{
	Elf64_Ehdr *ehdr = (Elf64_Ehdr *)file->addr;
	Elf64_Phdr *phdr = (Elf64_Phdr *)((char *)file->addr + ehdr->e_phoff);
	Elf64_Half phnum = ehdr->e_phnum;

	int increase = 0;
	while (phnum--) {
		if (increase) {
			phdr->p_offset += 0x1000;
			phdr->p_vaddr += 0x1000;
		}
		if (phdr->p_type == PT_LOAD && phdr->p_flags & PF_X) {
			/* phdr->p_filesz += 0x1000; */
			/* phdr->p_memsz += 0x1000; */
			increase = 1;
		}
		phdr++;
	}
	return (0);
}

int woody64(struct file_info *file)
{
	if (woody64_check_ehdr(file))
		return (1);
	if (woody64_insert_page(file))
		return (1);
	return (0);
}
