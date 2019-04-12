#ifndef PACKER_H
# define PACKER_H

# include <stddef.h>
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
map_file(struct elf_info *file);

int
unmap_file(struct elf_info *file);

#endif /* PACKER_H */
