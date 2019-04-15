#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include "packer.h"

static void close_fd(int *fd) {close(*fd);}

static int
elf_ident(int fd, struct elf_info *bin)
{
	unsigned char ident[EI_NIDENT];
	unsigned char elfmag[] = {ELFMAG0, ELFMAG1, ELFMAG2, ELFMAG3};

	if (read(fd, ident, EI_NIDENT) < EI_NIDENT)
		return (error(ERR_EI_NIDENT, bin->name));
	if (ft_memcmp(ident, elfmag, sizeof(elfmag)))
		return (error(ERR_EI_ELFMAG, bin->name));
	if (ident[EI_CLASS] != ELFCLASS32 && ident[EI_CLASS] != ELFCLASS64)
		return (error(ERR_EI_CLASS, bin->name));
	bin->class = ident[EI_CLASS];
	if (lseek(fd, 0, SEEK_SET) != 0)
		return (error(ERR_LSEEK, bin->name));
	return (0);
}

static int
map(struct elf_info *bin)
{
	int fd __attribute__ ((cleanup (close_fd)));
	struct stat buf;

	if ((fd = open(bin->name, O_RDONLY)) < 0)
		return (error(ERR_OPEN, bin->name));
	if (elf_ident(fd, bin))
		return (1);
	if (fstat(fd, &buf) < 0)
		return (error(ERR_STAT, bin->name));
	bin->addr = mmap(NULL, buf.st_size, PROT_READ | PROT_WRITE,
		MAP_PRIVATE, fd, 0);
	if (bin->addr == MAP_FAILED)
		return (error(ERR_MMAP, bin->name));
	bin->length = buf.st_size;
	return (0);
}

static int
woody(struct elf_info *bin)
{
	int fd __attribute__ ((cleanup (close_fd)));
	ssize_t bytes;

	if ((fd = open("woody", O_WRONLY | O_CREAT | O_TRUNC, 0755)) < 0)
		return (error(ERR_OPEN, "woody"));
	bytes = write(fd, bin->addr, bin->length);
	if (bytes == -1 || (size_t)bytes < bin->length)
		return (error(ERR_WRITE, "woody"));
	return (0);
}

int
packer(const char *filename)
{
	struct elf_info bin;
	int ret;

	bin.name = filename;
	if (map(&bin))
		return (1);
	if (bin.class == ELFCLASS64)
		ret = packer64(&bin);
	else
		ret = packer32(&bin);
	if (ret == 0)
		ret = woody(&bin);
	if (munmap(bin.addr, bin.length) < 0)
		return (error(ERR_MUNMAP, bin.name));
	return (ret);
}
