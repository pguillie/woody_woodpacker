#ifndef STRUCT_H
# define STRUCT_H

struct elf_info {
	const char *name;
	void *addr;
	size_t length;
	void *xseg;
};

#endif /* STRUCT_H */
