NAME	= woody_woodpacker
CC	= gcc
CFLAGS	= -Wall -Wextra -Werror -Iincludes -I$(dir $(LIBFT))includes

LIBFT	= libftasm/libfts.a

INCLUDES = $(addprefix includes/, \
	err_fct.h \
	error.h \
	packer.h \
	program.h \
	struct.h \
)
SOURCES = $(addprefix sources/, \
	err_fct.c \
	error.c \
	main.c \
	packer.c \
	packer32.c \
	packer64.c \
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
