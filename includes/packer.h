#ifndef PACKER_H
# define PACKER_H

# include <stddef.h>
# include "libfts.h"

# include "error.h"
# include "struct.h"

int packer(const char *filename);
int packer32(struct elf_info *bin);
int packer64(struct elf_info *bin);

int elf_ident(const char *filename, unsigned char *class);

int map_file(struct elf_info *file);
int unmap_file(struct elf_info *file);

int write_woody(struct elf_info *bin);

#endif /* PACKER_H */
