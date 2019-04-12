#ifndef STRUCT_H
# define STRUCT_H

struct elf_info {
	const char *name;
	void *addr;
	size_t length;
	unsigned char class;
};

#endif /* STRUCT_H */
