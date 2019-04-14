#include <elf.h>

#include "packer.h"

#define STUB_BEG (0x07)
#define STUB_LEN (0x17)
#define STUB_JMP (0x47)

#define TEXT_OFF (0x07)
#define TEXT_LEN (0x17)
#define JMP_OLD (0x47)

char stub[] = {
	0x50, 0x57, 0x56, 0x52, 0x48, 0x8d, 0x3d, 0xf5,
	0xff, 0xff, 0xff, 0x48, 0x31, 0xc0, 0x80, 0x34,
	0x07, 0x42, 0x48, 0xff, 0xc0, 0x48, 0x3d, 0x42,
	0x42, 0x42, 0x42, 0x7c, 0xf1, 0xe8, 0x0e, 0x00,
	0x00, 0x00, 0x2e, 0x2e, 0x2e, 0x2e, 0x57, 0x4f,
	0x4f, 0x44, 0x59, 0x2e, 0x2e, 0x2e, 0x2e, 0x0a,
	0xb8, 0x01, 0x00, 0x00, 0x00, 0xbf, 0x01, 0x00,
	0x00, 0x00, 0x5e, 0xba, 0x0e, 0x00, 0x00, 0x00,
	0x0f, 0x05, 0x5a, 0x5e, 0x5f, 0x58, 0xe9, 0x42,
	0x42, 0x42, 0x42
};

static int
check_ehdr(Elf64_Ehdr *ehdr, size_t length)
{
	if (sizeof(Elf64_Ehdr) > length)
		return (error(ERR_EHDR, 0));
	if (ehdr->e_type != ET_EXEC && ehdr->e_type != ET_DYN)
		return (error(ERR_E_TYPE, 0));
	if (ehdr->e_machine != EM_X86_64)
		return (error(ERR_E_MACHINE, 0));
	if (ehdr->e_entry == 0)
		return (error(ERR_E_ENTRY, 0));
	if (ehdr->e_phoff == 0)
		return (error(ERR_E_PHOFF, 0));
	return (0);
}

static int
get_exec_phdr(Elf64_Ehdr *ehdr, Elf64_Phdr **phdr, struct elf_info *bin)
{
	uint16_t phnum;

	if (ehdr->e_phoff == 0 || ehdr->e_phnum == 0)
		return (error(ERR_PHDR, 0));
	*phdr = (Elf64_Phdr *)((char *)ehdr + ehdr->e_phoff);
	phnum = ehdr->e_phnum;
	if ((char *)(*phdr) + (phnum * sizeof(Elf64_Phdr))
		> (char *)bin->addr + bin->length)
		return (error(ERR_CORRUPT, 0));
	while (phnum-- && !((*phdr)->p_type == PT_LOAD && (*phdr)->p_flags & PF_X))
		(*phdr)++;
	if ((*phdr)->p_type != PT_LOAD || !((*phdr)->p_flags & PF_X))
		return (error(ERR_PHDR, 0));
	if ((*phdr)->p_offset + (*phdr)->p_filesz > bin->length)
		return (error(ERR_CORRUPT, 0));
	return (0);
	/* while (phnum--) { */
	/* 	if ((*phdr)->p_type == PT_LOAD && (*phdr)->p_flags & PF_X) { */
	/* 		if ((*phdr)->p_offset + (*phdr)->p_filesz > bin->length) */
	/* 			return (error(ERR_CORRUPT, 0)); */
	/* 		return (0); */
	/* 	} */
	/* 	(*phdr)++; */
	/* } */
	/* return (error(ERR_PHDR, 0)); */
}

static int
get_strtab(Elf64_Ehdr *ehdr, Elf64_Shdr **strtab, struct elf_info *bin)
{
	uint16_t shnum;

	*strtab = (Elf64_Shdr *)((char *)ehdr + ehdr->e_shoff);
	shnum = ehdr->e_shnum;
	while (shnum--) {
		if ((*strtab)->sh_type == SHT_STRTAB) {
			if ((*strtab)->sh_offset + (*strtab)->sh_size
				> bin->length)
				return (error(ERR_CORRUPT, "AAA"));
			if (((char *)bin->addr + (*strtab)->sh_offset)[(*strtab)->sh_size - 1] != '\0')
				return (error(ERR_CORRUPT, "BBB"));
			if (ft_strcmp((char *)bin->addr + (*strtab)->sh_offset + (*strtab)->sh_name, ".shstrtab") == 0)
				return (0);
		}
		(*strtab)++;
	}
	return (error(ERR_SHDR, ".shstrtab"));

	/* if ((*strtab)->sh_type != SHT_STRTAB) */
	/* 	return (error(ERR_SHDR, "strtab")); */
	/* if ((*strtab)->sh_offset + (*strtab)->sh_size > bin->length) */
	/* 	return (error(ERR_CORRUPT, 0)); */
	/* if (*((char *)bin->addr + (*strtab)->sh_offset + (*strtab)->sh_size) */
	/* 	!= '\0') */
	/* 	return (error(ERR_CORRUPT, 0)); */
	/* return (0); */
}

static int
get_text_shdr(Elf64_Ehdr *ehdr, Elf64_Shdr **shdr, struct elf_info *bin)
{
	Elf64_Shdr *strtab;
	uint16_t shnum;

	if (ehdr->e_shoff == 0 || ehdr->e_shnum == 0)
		return (error(ERR_SHDR, ".text"));
	*shdr = (Elf64_Shdr *)((char *)ehdr + ehdr->e_shoff);
	shnum = ehdr->e_shnum;
	if ((char *)(*shdr) + (shnum * sizeof(Elf64_Shdr))
		> (char *)bin->addr + bin->length)
		return (error(ERR_CORRUPT, 0));
	if (get_strtab(ehdr, &strtab, bin))
		return (1);
	while (shnum--) {
		if ((*shdr)->sh_name > strtab->sh_size)
			return (error(ERR_CORRUPT, 0));
		if (ft_strcmp((char *)bin->addr + strtab->sh_offset
				+ (*shdr)->sh_name, ".text") == 0) {
			if ((*shdr)->sh_offset + (*shdr)->sh_size > bin->length)
				return (error(ERR_CORRUPT, 0));
			if (!((*shdr)->sh_flags & (SHF_ALLOC | SHF_EXECINSTR)))
				return (error(ERR_TEXT, "'AX' flags"));
			if ((*shdr)->sh_type != SHT_PROGBITS)
				return (error(ERR_TEXT, "'PROGBITS' type"));
			return (0);
		}
		(*shdr)++;
	}
	return (error(ERR_SHDR, ".text"));
}

static int
patch(Elf64_Ehdr *ehdr, Elf64_Phdr *phdr, Elf64_Shdr *shdr)
{
	/* *(uint32_t *)(stub + TEXT_OFF) = (uint32_t)(shdr->sh_offset */
	/* 	- (phdr->p_vaddr + phdr->p_memsz + TEXT_OFF + 4)); */
	/* *(uint32_t *)(stub + TEXT_LEN) = (uint32_t)(shdr->sh_size); */
	/* *(uint32_t *)(stub + JMP_OLD) = (uint32_t)(ehdr->e_entry */
	/* 	- (phdr->p_vaddr + phdr->p_memsz + JMP_OLD + 4)); */
	(void)shdr;

	*(uint32_t *)(stub + STUB_BEG) = (uint32_t)(-phdr->p_memsz
		- (STUB_BEG + 4));
	*(uint32_t *)(stub + STUB_LEN) = (uint32_t)(phdr->p_filesz);
	*(uint32_t *)(stub + STUB_JMP) = (uint32_t)(ehdr->e_entry
		- phdr->p_vaddr - phdr->p_memsz - (STUB_JMP + 4));
	return (0);
}

static int
encrypt(void *addr, size_t length)
{
	while (length--)
		*(char *)(addr + length) ^= 0x42;
	return (0);
}

static int
inject(Elf64_Phdr *phdr, struct elf_info *bin)
{
	Elf64_Ehdr *ehdr = (Elf64_Ehdr *)bin->addr;

	if (((phdr->p_filesz + phdr->p_align - 1) & ~(phdr->p_align - 1))
		- phdr->p_filesz < sizeof(stub))
		return (error(ERR_SPACE, bin->name));
	ft_memcpy((char *)bin->addr + phdr->p_offset + phdr->p_filesz, stub,
		sizeof(stub));
	ehdr->e_entry = phdr->p_vaddr + phdr->p_memsz;
	phdr->p_filesz += sizeof(stub);
	phdr->p_memsz += sizeof(stub);
	phdr->p_flags |= PF_W;
	return (0);
}

int
packer64(struct elf_info *bin)
{
	Elf64_Ehdr *ehdr;
	Elf64_Phdr *phdr;
	Elf64_Shdr *shdr;

	ehdr = (Elf64_Ehdr *)bin->addr;
	if (check_ehdr(ehdr, bin->length))
		return (1);
	if (get_exec_phdr(ehdr, &phdr, bin))
		return (1);
	if (get_text_shdr(ehdr, &shdr, bin))
		return (1);
	patch(ehdr, phdr, shdr); //
	//generate key from stub
	//API to get text section and call a real encryption algorithm
	if (encrypt(bin->addr + phdr->p_offset, phdr->p_filesz))
		return (1);
	if (inject(phdr, bin))
		return (1);
	return (0);
}
