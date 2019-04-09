#include <elf.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

#include "packer.h"

int write_woody(struct elf_info *bin)
{
	struct elf_info woody;
	Elf64_Phdr *xseg = bin->xseg;

	woody.name = "woody";
	woody.length = bin->length + 0x1000;
	woody.addr = mmap(NULL, woody.length, PROT_READ | PROT_WRITE,
		MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (woody.addr == MAP_FAILED)
		return (error(ERR_MMAP, woody.name));
	uint64_t align = ((xseg->p_filesz + xseg->p_align - 1)
		& ~(xseg->p_align - 1)) - xseg->p_filesz;
	Elf64_Off i;
	for (i = 0; i < xseg->p_offset + xseg->p_filesz; i++)
		((unsigned char *)woody.addr)[i]
			= ((unsigned char *)bin->addr)[i];
	ft_memset(woody.addr + i, 0, 0x1000 + align);
	for (i += align; i < bin->length; i++)
		((unsigned char *)woody.addr)[i + 0x1000]
			= ((unsigned char *)bin->addr)[i];
	int fd = open(woody.name, O_WRONLY | O_CREAT | O_TRUNC, 0755);
	if (fd < 0)
		return (error(ERR_OPEN, woody.name));
	ssize_t bytes = write(fd, woody.addr, woody.length);
	if (bytes < 0 || (size_t)bytes < woody.length)
		return (error(ERR_WRITE, woody.name));
	close(fd);
	unmap_file(&woody);
	return (0);
}
