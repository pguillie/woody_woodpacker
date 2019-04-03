#ifndef WOODY_ERROR_H
# define WOODY_ERROR_H

# define WE_PREF "woody_woodpacker: "

typedef enum {
	WE_E_ENTRY,
	WE_E_MACHINE,
	WE_E_PHOFF,
	WE_E_TYPE,
	WE_EI_CLASS,
	WE_EI_CLASS32, //tmp
	WE_EI_ELFMAG,
	WE_EI_NIDENT,
	WE_MMAP,
	WE_MUNMAP,
	WE_OPEN,
	WE_STAT,
	WE_WRITE,
	WE_NUMBER,
} we_id;

typedef int (*we_fct)(const char *);

int woody_error(we_id id, const char *file);

int we_e_entry(const char *file);
int we_e_machine(const char *file);
int we_e_phoff(const char *file);
int we_e_type(const char *file);
int we_ei_class(const char *file);
int we_ei_class32(const char *null __attribute__ ((unused))); //tmp
int we_ei_elfmag(const char *file);
int we_ei_nident(const char *file);
int we_mmap(const char *file);
int we_munmap(const char *null __attribute__ ((unused)));
int we_open(const char *file);
int we_stat(const char *file);
int we_write(const char *file);

#endif /* WOODY_ERROR_H */
