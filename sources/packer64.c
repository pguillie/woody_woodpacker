#include <elf.h>

#include "packer.h"

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

#define STUB_BEG (0x07)
#define STUB_LEN (0x17)
#define STUB_JMP (0x47)

static int
check_ehdr(struct elf_info *bin)
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

static Elf64_Phdr *
get_xseg(struct elf_info *bin)
{
	Elf64_Ehdr *ehdr = (Elf64_Ehdr *)bin->addr;
	Elf64_Phdr *phdr = (Elf64_Phdr *)((char *)bin->addr + ehdr->e_phoff);
	Elf64_Half phnum = ehdr->e_phnum;

	while (phnum-- && !(phdr->p_type == PT_LOAD && phdr->p_flags & PF_X))
		phdr++;
	return (phdr);
}

static int
encrypt(void *addr, size_t len)
{
	while (len--)
		((char *)addr)[len] ^= 0x42;
	return (0);
}

static int
inject(struct elf_info *bin)
{
	Elf64_Ehdr *ehdr = (Elf64_Ehdr *)bin->addr;
	Elf64_Phdr *phdr = get_xseg(bin);

	if ((((phdr->p_filesz + phdr->p_align - 1) & ~(phdr->p_align - 1))
			- phdr->p_filesz) < sizeof(stub))
		return (error(ERR_SPACE, bin->name));
	*(uint32_t *)(stub + STUB_BEG) = (uint32_t)(- phdr->p_memsz
		- (STUB_BEG + 4));
	*(uint32_t *)(stub + STUB_LEN) = (uint32_t)(phdr->p_filesz);
	*(uint32_t *)(stub + STUB_JMP) = (uint32_t)(ehdr->e_entry
		- phdr->p_vaddr - phdr->p_memsz - (STUB_JMP + 4));
	encrypt(bin->addr + phdr->p_offset, phdr->p_filesz);
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
	if (check_ehdr(bin))
		return (1);
	if (inject(bin))
		return (1);
	if (write_woody(bin))
		return (1);
	return (0);
}
