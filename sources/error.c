#include <stdio.h>

#include "packer.h"
#include "err_fct.h"

err_fct fct[ERR_NUMBER] = {
	err_corrupt,
	err_e_entry,
	err_e_machine,
	err_e_phoff,
	err_e_type,
	err_ei_class,
	err_ei_class32,
	err_ei_elfmag,
	err_ei_nident,
	err_ehdr,
	err_lseek,
	err_mmap,
	err_munmap,
	err_open,
	err_phdr,
	err_space,
	err_stat,
	err_usage,
	err_write,
};

int
error(err_id id, const char *file)
{
	if (id < ERR_NUMBER)
		return (fct[id](file));
	fprintf(stderr, "Error: UNKNOWN. You should not see this.\n");
	return (1);
}
