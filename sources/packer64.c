#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "packer.h"

#define PARASITE_SIZE ((uint64_t)parasite_end - (uint64_t)parasite)

#define PARASITE_ENTRY (0x11e)
#define PARA_LEN ((PARASITE_ENTRY) + 8)
#define TEXT_OFF ((PARASITE_ENTRY) + 20)
#define TEXT_LEN ((PARASITE_ENTRY) + 25)
#define JUMP_OLD ((PARASITE_ENTRY) + 84)

static int
check_ehdr(Elf64_Ehdr *ehdr, size_t length)
{
	if (sizeof(Elf64_Ehdr) > length)
		return (error(ERR_CORRUPT, "ELF header"));
	if (ehdr->e_type != ET_EXEC && ehdr->e_type != ET_DYN)
		return (error(ERR_EHDR, "executable type (EXEC or DYN)"));
	if (ehdr->e_machine != EM_X86_64)
		return (error(ERR_EHDR, "machine architecture (X86_64)"));
	if (ehdr->e_entry == 0)
		return (error(ERR_EHDR, "entry point"));
	if (ehdr->e_phoff == 0 || ehdr->e_phnum == 0)
		return (error(ERR_EHDR, "program header table"));
	if (ehdr->e_shoff == 0 || ehdr->e_shnum == 0)
		return (error(ERR_EHDR, "section header table"));
	return (0);
}

static int
patch(Elf64_Ehdr *ehdr, Elf64_Phdr *phdr, Elf64_Shdr *shdr)
{
	*(uint32_t *)(parasite + PARA_LEN) = (uint32_t)(PARASITE_SIZE);
	*(uint32_t *)(parasite + TEXT_OFF) = (uint32_t)(shdr->sh_offset
		- (phdr->p_offset + phdr->p_filesz + TEXT_OFF + 4));
	*(uint32_t *)(parasite + TEXT_LEN) = (uint32_t)(shdr->sh_size);
	*(uint32_t *)(parasite + JUMP_OLD) = (uint32_t)(ehdr->e_entry
		- (phdr->p_vaddr + phdr->p_memsz + JUMP_OLD + 4));
	return (0);
}

static int
inject(Elf64_Ehdr *ehdr, Elf64_Phdr *phdr, struct elf_info *bin)
{
	if (((phdr->p_filesz + phdr->p_align - 1) & ~(phdr->p_align - 1))
		- phdr->p_filesz < PARASITE_SIZE)
		return (error(ERR_SPACE, bin->name));
	ft_memcpy((char *)bin->addr + phdr->p_offset + phdr->p_filesz,
		parasite, PARASITE_SIZE);
	ehdr->e_entry = phdr->p_vaddr + phdr->p_memsz;
	phdr->p_filesz += PARASITE_SIZE;
	phdr->p_memsz += PARASITE_SIZE;
	phdr->p_flags |= PF_W;
	return (0);
}

int
packer64(struct elf_info *bin)
{
	Elf64_Ehdr *ehdr;
	Elf64_Phdr *phdr;
	Elf64_Shdr *shdr;
	uint64_t key;

	ehdr = (Elf64_Ehdr *)bin->addr;
	if (check_ehdr(ehdr, bin->length))
		return (1);
	if (get_exec_phdr64(ehdr, &phdr, bin))
		return (1);
	if (get_text_shdr64(ehdr, &shdr, bin))
		return (1);
	if (mprotect((void *)((uint64_t)parasite & (uint64_t)-getpagesize()),
			PARASITE_SIZE, PROT_READ | PROT_WRITE | PROT_EXEC))
		return (error(ERR_PROT, "parasite"));
	patch(ehdr, phdr, shdr);
	key = hash(parasite, PARASITE_SIZE);
	printf("encryption key: %#.16lx\n", key);
	encrypt(bin->addr + shdr->sh_offset, shdr->sh_size, key);
	if (inject(ehdr, phdr, bin))
		return (1);
	return (0);
}
