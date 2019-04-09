NAME	= woody_woodpacker
CC	= gcc
CFLAGS	= -Wall -Wextra -Werror -Iincludes -I$(dir $(LIBFT))includes -g3 -fsanitize=address

LIBFT	= libftasm/libfts.a

INCLUDES = $(addprefix includes/, \
	err_fct.h \
	error.h \
	packer.h \
	struct.h \
)
SOURCES = $(addprefix sources/, \
	main.c \
	packer.c \
	packer32.c \
	packer64.c \
	elf_ident.c \
	err_fct.c \
	error.c \
	mapping.c \
	write_woody.c \
)
OBJECTS = $(SOURCES:%.c=%.o)

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(LIBFT) $(OBJECTS)
	$(CC) -g3 -fsanitize=address -o $@ $^ $(LIBFT)

$(OBJECTS): $(INCLUDES) Makefile

$(LIBFT):
	make -C $(dir $@)

clean:
	make -C $(dir $(LIBFT)) fclean
	rm -fv $(OBJECTS)

fclean: clean
	rm -fv $(NAME)

re: clean all
