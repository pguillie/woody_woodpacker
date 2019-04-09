#ifndef ERROR_H
# define ERROR_H

typedef enum {
	ERR_E_ENTRY,
	ERR_E_MACHINE,
	ERR_E_PHOFF,
	ERR_E_TYPE,
	ERR_EI_CLASS,
	ERR_EI_CLASS32, //tmp
	ERR_EI_ELFMAG,
	ERR_EI_NIDENT,
	ERR_MMAP,
	ERR_MUNMAP,
	ERR_OPEN,
	ERR_SPACE,
	ERR_STAT,
	ERR_WRITE,
	ERR_NUMBER,
} err_id;

int error(err_id id, const char *file);

#endif /* ERROR_H */
