#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include "woody.h"

static void close_fd(int *fd)
{
	close(*fd);
}

int woody_map_file(struct file_info *file)
{
	int fd __attribute__ ((cleanup (close_fd)));
	struct stat buf;

	fd = open(file->name, O_RDONLY);
	if (fd < 0)
		return (woody_error(WE_OPEN, file->name));
	if (fstat(fd, &buf) < 0)
		return (woody_error(WE_STAT, file->name));
	file->addr = mmap(NULL, buf.st_size, PROT_READ | PROT_WRITE,
		MAP_PRIVATE, fd, 0);
	if (file->addr == MAP_FAILED)
		return (woody_error(WE_MMAP, file->name));
	file->length = buf.st_size;
	return (0);
}

int woody_unmap_file(struct file_info *file)
{
	if (munmap(file->addr, file->length) < 0)
		return (woody_error(WE_MUNMAP, file->name));
	file->addr = NULL;
	file->length = 0;
	return (0);
}
