#include <elf.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

#include "packer.h"

int write_woody(struct elf_info *bin)
{
	int fd = open("woody", O_WRONLY | O_CREAT | O_TRUNC, 0755);
	if (fd < 0)
		return (error(ERR_OPEN, "woody"));
	ssize_t bytes = write(fd, bin->addr, bin->length);
	if (bytes < 0 || (size_t)bytes < bin->length)
		return (error(ERR_WRITE, "woody"));
	close(fd);
	return (0);
}
