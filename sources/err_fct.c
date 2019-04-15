#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "program.h"

char *program;

int
err_corrupt(const char *elem)
{
	fprintf(stderr, "%s: corrupted, truncated or malformed file:"
		"t%s extends past end-of-file\n", program, elem);
	return (1);
}

int
err_ei_class(const char *file)
{
	fprintf(stderr, "%s: '%s' has no valid ELF architecture class\n",
		program, file);
	return (1);
}

int
err_ei_class32(const char *null __attribute__ ((unused)))
{
	fprintf(stderr, "%s: 32-bit architecture is not supported yet\n",
		program);
	return (1);
}

int
err_ei_elfmag(const char *file)
{
	fprintf(stderr, "%s: '%s' is not an ELF file\n", program, file);
	return (1);
}

int
err_ei_nident(const char *file)
{
	fprintf(stderr, "%s: failed to get '%s' identification\n", program,
		file);
	return (1);
}

int
err_ehdr(const char *member)
{
	fprintf(stderr, "%s: ELF header must specify a valid %s\n", program,
		member);
	return (1);
}

int
err_lseek(const char *file)
{
	fprintf(stderr, "%s: failed to reposition file offset to the "
		"beginning of '%s'\n", program, file);
	return (1);
}

int
err_mmap(const char *file)
{
	fprintf(stderr, "%s: failed to map '%s': %s\n", program, file,
		strerror(errno));
	return (1);
}

int
err_munmap(const char *file)
{
	fprintf(stderr, "%s: failed to unmap '%s': %s\n", program, file,
		strerror(errno));
	return (1);
}

int
err_open(const char *file)
{
	fprintf(stderr, "%s: failed to open '%s': %s\n", program, file,
		strerror(errno));
	return (1);
}

int
err_phdr(const char *seg)
{
	fprintf(stderr, "%s: %s segment not found\n", program, seg);
	return (1);
}

int
err_shdr(const char *sect)
{
	fprintf(stderr, "%s: %s section not found\n", program, sect);
	return (1);
}

int
err_space(const char *file __attribute__ ((unused)))
{
	fprintf(stderr, "%s: not enough space on text segment!\n",
		program);
	return (1);
}

int
err_stat(const char *file)
{
	fprintf(stderr, "%s: failed to stat '%s': %s\n", program, file,
		strerror(errno));
	return (1);
}

int
err_text(const char *member)
{
	fprintf(stderr, "%s: text section must have %s\n", program, member);
	return (1);
}

int
err_usage(const char *file __attribute__ ((unused)))
{
	fprintf(stderr, "Usage: %s FILE\n", program);
	return (1);
}

int
err_write(const char *file)
{
	fprintf(stderr, "%s: failed to write '%s' new binary\n", program, file);
	return (1);
}
