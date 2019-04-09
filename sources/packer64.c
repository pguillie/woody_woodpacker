#include <elf.h>

#include "packer.h"

#define JMP_OFF (48)

char hw[] = { //hello world
	0x50, 0x57, 0x56, 0x52, 0xe8, 0x0e, 0x00, 0x00,
	0x00, 0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x2c, 0x20,
	0x57, 0x6f, 0x72, 0x6c, 0x64, 0x21, 0x0a, 0xb8,
	0x01, 0x00, 0x00, 0x00, 0xbf, 0x01, 0x00, 0x00,
	0x00, 0x5e, 0xba, 0x0e, 0x00, 0x00, 0x00, 0x0f,
	0x05, 0x5a, 0x5e, 0x5f, 0x58, 0x90, 0x90, 0xe9,
	0x42, 0x42, 0x42, 0x42
};

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

static int inject(struct elf_info *bin)
{
	Elf64_Ehdr *ehdr = (Elf64_Ehdr *)bin->addr;
	Elf64_Phdr *phdr = (Elf64_Phdr *)((char *)bin->addr + ehdr->e_phoff);
	Elf64_Half phnum = ehdr->e_phnum;

	while (phnum-- && !(phdr->p_type == PT_LOAD && phdr->p_flags & PF_X))
		phdr++;
	if ((((phdr->p_filesz + phdr->p_align - 1) & ~(phdr->p_align - 1))
			- phdr->p_filesz) < sizeof(hw))
		error(ERR_SPACE, bin->name);//todo
	*(uint32_t *)(hw + JMP_OFF) = (uint32_t)(ehdr->e_entry - phdr->p_vaddr
		- phdr->p_memsz - sizeof(hw));
	ft_memcpy((char *)(bin->addr) + phdr->p_offset + phdr->p_filesz, hw,
		sizeof(hw));
	ehdr->e_entry = phdr->p_vaddr + phdr->p_memsz;
	phdr->p_filesz += sizeof(hw);
	phdr->p_memsz += sizeof(hw);
	return (0);
}

int packer64(struct elf_info *bin)
{
	if (check_ehdr(bin))
		return (1);
	if (inject(bin))
		return (1);
	if (write_woody(bin))
		return (1);
	return (0);
}