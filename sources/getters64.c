#include <packer.h>

/* executable segment getter */

int
get_exec_phdr64(Elf64_Ehdr *ehdr, Elf64_Phdr **phdr, struct elf_info *bin)
{
	uint16_t phnum;

	*phdr = (Elf64_Phdr *)((char *)ehdr + ehdr->e_phoff);
	phnum = ehdr->e_phnum;
	if ((char *)(*phdr) + (phnum * sizeof(Elf64_Phdr))
		> (char *)bin->addr + bin->length)
		return (error(ERR_CORRUPT, "program header table"));
	while (phnum--) {
		if ((*phdr)->p_type != PT_LOAD || !((*phdr)->p_flags & PF_X)) {
			(*phdr)++;
			continue ;
		}
		if ((*phdr)->p_offset + (*phdr)->p_filesz > bin->length)
			return (error(ERR_CORRUPT, "executable segment"));
		return (0);
	}
	return (error(ERR_PHDR, "executable"));
}

/* text section getter */

static int
get_shstrtab64(Elf64_Ehdr *ehdr, Elf64_Shdr **shstrtab, struct elf_info *bin)
{
	uint16_t shnum;

	*shstrtab = (Elf64_Shdr *)((char *)ehdr + ehdr->e_shoff);
	shnum = ehdr->e_shnum;
	while (shnum--) {
		if ((*shstrtab)->sh_type != SHT_STRTAB) {
			(*shstrtab)++;
			continue ;
		}
		if ((*shstrtab)->sh_offset + (*shstrtab)->sh_size > bin->length)
			return (error(ERR_CORRUPT, "strtab section"));
		if (((char *)bin->addr + (*shstrtab)->sh_offset)
			[(*shstrtab)->sh_size - 1] != '\0')
			return (error(ERR_CORRUPT, "strtab section"));
		if (ft_strcmp((char *)bin->addr + (*shstrtab)->sh_offset
				+ (*shstrtab)->sh_name, ".shstrtab") == 0)
			return (0);
		(*shstrtab)++;
	}
	return (error(ERR_SHDR, ".shstrtab"));
}

int
get_text_shdr64(Elf64_Ehdr *ehdr, Elf64_Shdr **shdr, struct elf_info *bin)
{
	Elf64_Shdr *shstrtab;
	uint16_t shnum;

	*shdr = (Elf64_Shdr *)((char *)ehdr + ehdr->e_shoff);
	shnum = ehdr->e_shnum;
	if ((char *)(*shdr) + (shnum * sizeof(Elf64_Shdr))
		> (char *)bin->addr + bin->length)
		return (error(ERR_CORRUPT, "section header table"));
	if (get_shstrtab64(ehdr, &shstrtab, bin))
		return (1);
	while (shnum--) {
		if ((*shdr)->sh_name > shstrtab->sh_size)
			return (error(ERR_CORRUPT, "section name"));
		if (ft_strcmp((char *)bin->addr + shstrtab->sh_offset
				+ (*shdr)->sh_name, ".text")) {
			(*shdr)++;
			continue ;
		}
		if ((*shdr)->sh_offset + (*shdr)->sh_size > bin->length)
			return (error(ERR_CORRUPT, 0));
		if (!((*shdr)->sh_flags & (SHF_ALLOC | SHF_EXECINSTR)))
			return (error(ERR_TEXT, "'AX' flags"));
		if ((*shdr)->sh_type != SHT_PROGBITS)
			return (error(ERR_TEXT, "'PROGBITS' type"));
		return (0);
	}
	return (error(ERR_SHDR, ".text"));
}
