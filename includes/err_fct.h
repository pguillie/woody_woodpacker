#ifndef ERR_FCT_H
# define ERR_FCT_H

typedef int (*err_fct)(const char *);

int
err_corrupt(const char *file);

int
err_e_entry(const char *file);

int
err_e_machine(const char *file);

int
err_e_phoff(const char *file);

int
err_e_type(const char *file);

int
err_ei_class(const char *file);

int
err_ei_class32(const char *file); //tmp

int
err_ei_elfmag(const char *file);

int
err_ei_nident(const char *file);

int
err_ehdr(const char *file);

int
err_lseek(const char *file);

int
err_mmap(const char *file);

int
err_munmap(const char *file);

int
err_open(const char *file);

int
err_phdr(const char *file);

int
err_shdr(const char *sect);

int
err_space(const char *file); //tmp

int
err_stat(const char *file);

int
err_text(const char *member);

int
err_usage(const char *file);

int
err_write(const char *file);

#endif /* ERR_FCT_H */
