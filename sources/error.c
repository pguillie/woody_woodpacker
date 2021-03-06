#include <stdio.h>

#include "packer.h"
#include "err_fct.h"

err_fct fct[ERR_NUMBER] = {
	err_corrupt,
	err_ei_class,
	err_ei_class32,
	err_ei_elfmag,
	err_ei_nident,
	err_ehdr,
	err_lseek,
	err_mmap,
	err_munmap,
	err_open,
	err_packed,
	err_phdr,
	err_prot,
	err_shdr,
	err_space,
	err_stat,
	err_text,
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
