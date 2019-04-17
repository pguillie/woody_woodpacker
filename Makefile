NAME	= woody_woodpacker
CC	= gcc
AS	= nasm
CFLAGS	= -Wall -Wextra -Werror -Iincludes -I$(dir $(LIBFT))includes -g3 -fsanitize=address
ASFLAGS	= -felf64

LIBFT	= libftasm/libfts.a

INCLUDES = $(addprefix includes/, \
	err_fct.h \
	error.h \
	packer.h \
	program.h \
	struct.h \
)

C_SOURCES = \
	err_fct.c \
	error.c \
	getters64.c \
	main.c \
	packer.c \
	packer32.c \
	packer64.c \

ASM_SOURCES = \
	parasite.s \

OBJECTS = $(addprefix sources/, \
	$(ASM_SOURCES:%.s=%.o) \
	$(C_SOURCES:%.c=%.o) \
)

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(LIBFT) $(OBJECTS)
	$(CC) -o $@ $^ $(LIBFT) -fsanitize=address

$(OBJECTS): $(INCLUDES) Makefile

$(LIBFT):
	make -C $(dir $@)

clean:
	make -C $(dir $(LIBFT)) fclean
	rm -fv $(OBJECTS)

fclean: clean
	rm -fv $(NAME)

re: clean all
