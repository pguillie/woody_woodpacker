#ifndef WOODY_H
# define WOODY_H

# include <stddef.h>
# include "libfts.h"

# include "woody_error.h"
# include "woody_struct.h"

int woody(const char *filename);
int woody32(struct file_info *file);
int woody64(struct file_info *file);
int woody_elf_ident(const char *filename, unsigned char *class);
int woody_map_file(struct file_info *file);
int woody_unmap_file(struct file_info *file);
int woody_write_binary(struct file_info *file);

#endif /* WOODY_H */
