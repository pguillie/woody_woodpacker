#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "woody.h"

int woody_error(we_id id, const char *file)
{
	we_fct fct[WE_NUMBER] = {
		we_e_entry,
		we_e_machine,
		we_e_phoff,
		we_e_type,
		we_ei_class,
		we_ei_class32,
		we_ei_elfmag,
		we_ei_nident,
		we_mmap,
		we_munmap,
		we_open,
		we_stat,
		we_write,
	};

	if (id < WE_NUMBER)
		return (fct[id](file));
	fprintf(stderr, WE_PREF "Error: UNKNOWN. You should not see this.\n");
	return (1);
}

/* error functions */

int we_e_entry(const char *file)
{
	fprintf(stderr, WE_PREF "'%s' has no valid entry point\n", file);
	return (1);
}

int we_e_machine(const char *file)
{
	fprintf(stderr, WE_PREF "'%s' is not made for the current"
		"architecture\n", file);
	return (1);
}

int we_e_phoff(const char *file)
{
	fprintf(stderr, WE_PREF "'%s' has no program header table\n", file);
	return (1);
}

int we_e_type(const char *file)
{
	fprintf(stderr, WE_PREF "'%s' is not an executable file\n", file);
	return (1);
}

int we_ei_class(const char *file)
{
	fprintf(stderr, WE_PREF "'%s' has no valid ELF architecture class\n",
		file);
	return (1);
}

int we_ei_class32(const char *null __attribute__ ((unused))) //tmp
{
	fprintf(stderr, WE_PREF "32-bit architecture is not supported yet\n");
	return (1);
}

int we_ei_elfmag(const char *file)
{
	fprintf(stderr, WE_PREF "'%s' is not an ELF file\n", file);
	return (1);
}

int we_ei_nident(const char *file)
{
	fprintf(stderr, WE_PREF "failed to get '%s' identification\n", file);
	return (1);
}

int we_mmap(const char *file)
{
	fprintf(stderr, WE_PREF "failed to map '%s': %s\n", file,
		strerror(errno));
	return (1);
}

int we_munmap(const char *file)
{
	fprintf(stderr, WE_PREF "failed to unmap '%s': %s\n", file,
		strerror(errno));
	return (1);
}

int we_open(const char *file)
{
	fprintf(stderr, WE_PREF "failed to open '%s': %s\n", file,
		strerror(errno));
	return (1);
}

int we_stat(const char *file)
{
	fprintf(stderr, WE_PREF "failed to stat '%s': %s\n", file,
		strerror(errno));
	return (1);
}

int we_write(const char *file)
{
	fprintf(stderr, WE_PREF "failed to write '%s' new binary\n", file);
	return (1);
}
