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

void
parasite(void);

uint64_t
hash(const void *dat, size_t size);

void
encrypt(void *addr, size_t length, uint64_t key);

void
parasite_end(void);

#endif /* PACKER_H */
