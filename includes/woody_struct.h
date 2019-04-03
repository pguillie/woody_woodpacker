#ifndef WOODY_STRUCT_H
# define WOODY_STRUCT_H

struct file_info {
	const char *name;
	void *addr;
	size_t length;
};

#endif /* WOODY_STRUCT_H */
