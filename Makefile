NAME	= woody_woodpacker
CC	= gcc
CFLAGS	= -Wall -Wextra -Werror -Iincludes -I$(dir $(LIBFT))includes

LIBFT	= libftasm/libfts.a

INCLUDES = $(addprefix includes/, \
	woody.h \
	woody_error.h \
)
SOURCES = $(addprefix sources/, \
	main.c \
	woody.c \
	woody32.c \
	woody64.c \
	woody_elf_ident.c \
	woody_error.c \
	woody_mapping.c \
	woody_write_binary.c \
)
OBJECTS = $(SOURCES:%.c=%.o)

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(LIBFT) $(OBJECTS)
	$(CC) -o $@ $^ $(LIBFT)

$(OBJECTS): $(INCLUDES) Makefile

$(LIBFT):
	make -C $(dir $@)

clean:
	make -C $(dir $(LIBFT)) fclean
	rm -fv $(OBJECTS)

fclean: clean
	rm -fv $(NAME)

re: clean all
