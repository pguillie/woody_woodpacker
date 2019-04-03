#include <unistd.h>
#include <fcntl.h>

#include "woody.h"

int woody_write_binary(struct file_info *file)
{
	int fd;
	ssize_t bytes;

	fd = open("woody", O_WRONLY | O_CREAT, 0755);
	if (fd < 0)
		return (woody_error(WE_OPEN, "woody"));
	bytes = write(fd, file->addr, file->length);
	close(fd);
	if (bytes < 0 || (size_t)bytes < file->length)
		return (woody_error(WE_WRITE, "woody"));
	return (0);
}
