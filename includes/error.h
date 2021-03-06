#ifndef ERROR_H
# define ERROR_H

typedef enum {
	ERR_CORRUPT,
	ERR_EI_CLASS,
	ERR_EI_CLASS32, //tmp
	ERR_EI_ELFMAG,
	ERR_EI_NIDENT,
	ERR_EHDR,
	ERR_LSEEK,
	ERR_MMAP,
	ERR_MUNMAP,
	ERR_OPEN,
	ERR_PACKED,
	ERR_PHDR,
	ERR_PROT,
	ERR_SHDR,
	ERR_SPACE,
	ERR_STAT,
	ERR_TEXT,
	ERR_USAGE,
	ERR_WRITE,
	ERR_NUMBER,
} err_id;

int
error(err_id id, const char *file);

#endif /* ERROR_H */
