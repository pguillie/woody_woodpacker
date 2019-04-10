#include <stdio.h>
#include <string.h>
#include <errno.h>

#define ERR_PREF "woody_woodpacker: "

int
err_e_entry(const char *file)
{
	fprintf(stderr, ERR_PREF "'%s' has no valid entry point\n", file);
	return (1);
}

int
err_e_machine(const char *file)
{
	fprintf(stderr, ERR_PREF "'%s' is not made for the current"
		"architecture\n", file);
	return (1);
}

int
err_e_phoff(const char *file)
{
	fprintf(stderr, ERR_PREF "'%s' has no program header table\n", file);
	return (1);
}

int
err_e_type(const char *file)
{
	fprintf(stderr, ERR_PREF "'%s' is not an executable file\n", file);
	return (1);
}

int
err_ei_class(const char *file)
{
	fprintf(stderr, ERR_PREF "'%s' has no valid ELF architecture class\n",
		file);
	return (1);
}

int
err_ei_class32(const char *null __attribute__ ((unused)))
{
	fprintf(stderr, ERR_PREF "32-bit architecture is not supported yet\n");
	return (1);
}

int
err_ei_elfmag(const char *file)
{
	fprintf(stderr, ERR_PREF "'%s' is not an ELF file\n", file);
	return (1);
}

int
err_ei_nident(const char *file)
{
	fprintf(stderr, ERR_PREF "failed to get '%s' identification\n", file);
	return (1);
}

int
err_ehdr(const char *file)
{
	fprintf(stderr, ERR_PREF "failed to get '%s' ELF header\n", file);
	return (1);
}

int
err_mmap(const char *file)
{
	fprintf(stderr, ERR_PREF "failed to map '%s': %s\n", file,
		strerror(errno));
	return (1);
}

int
err_munmap(const char *file)
{
	fprintf(stderr, ERR_PREF "failed to unmap '%s': %s\n", file,
		strerror(errno));
	return (1);
}

int
err_open(const char *file)
{
	fprintf(stderr, ERR_PREF "failed to open '%s': %s\n", file,
		strerror(errno));
	return (1);
}

int
err_space(const char *file __attribute__ ((unused)))
{
	fprintf(stderr, ERR_PREF "not enough space on text segment!\n");
	return (1);
}

int
err_stat(const char *file)
{
	fprintf(stderr, ERR_PREF "failed to stat '%s': %s\n", file,
		strerror(errno));
	return (1);
}

int
err_write(const char *file)
{
	fprintf(stderr, ERR_PREF "failed to write '%s' new binary\n", file);
	return (1);
}
