#ifndef PACKER_H
# define PACKER_H

# include <elf.h>
# include "libfts.h"

# include "error.h"
# include "struct.h"

int
packer(const char *filename);

int
packer32(struct elf_info *bin);

int
packer64(struct elf_info *bin);

int
get_exec_phdr64(Elf64_Ehdr *ehdr, Elf64_Phdr **phdr, struct elf_info *bin);

int
get_text_shdr64(Elf64_Ehdr *ehdr, Elf64_Shdr **shdr, struct elf_info *bin);

int
map_file(struct elf_info *file);

int
unmap_file(struct elf_info *file);

#endif /* PACKER_H */
