#include "packer.h"

#define TEXT_OFF (0x07)
#define TEXT_LEN (0x17)
#define JMP_OLD (0x47)

char parasite[] = {
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
	*(uint32_t *)(parasite + TEXT_OFF) = (uint32_t)(shdr->sh_offset
		- (phdr->p_vaddr + phdr->p_memsz + TEXT_OFF + 4));
	*(uint32_t *)(parasite + TEXT_LEN) = (uint32_t)(shdr->sh_size);
	*(uint32_t *)(parasite + JMP_OLD) = (uint32_t)(ehdr->e_entry
		- (phdr->p_vaddr + phdr->p_memsz + JMP_OLD + 4));
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
inject(Elf64_Ehdr *ehdr, Elf64_Phdr *phdr, struct elf_info *bin)
{
	if (((phdr->p_filesz + phdr->p_align - 1) & ~(phdr->p_align - 1))
		- phdr->p_filesz < sizeof(parasite))
		return (error(ERR_SPACE, bin->name));
	ft_memcpy((char *)bin->addr + phdr->p_offset + phdr->p_filesz,
		parasite, sizeof(parasite));
	ehdr->e_entry = phdr->p_vaddr + phdr->p_memsz;
	phdr->p_filesz += sizeof(parasite);
	phdr->p_memsz += sizeof(parasite);
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
	if (get_exec_phdr64(ehdr, &phdr, bin))
		return (1);
	if (get_text_shdr64(ehdr, &shdr, bin))
		return (1);
	patch(ehdr, phdr, shdr); //
	//generate key from parasite
	//API to get text section and call a real encryption algorithm
	if (encrypt(bin->addr + shdr->sh_offset, shdr->sh_size))
		return (1);
	if (inject(ehdr, phdr, bin))
		return (1);
	return (0);
}
