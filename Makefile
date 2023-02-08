NAME	= ft_nm

SRC		= main.c \
			elf64.c elf32.c ar.c \
			utils.c

OBJ		= $(addprefix $(OBJDIR),$(SRC:.c=.o))

CC		= gcc -g
CFLAGS	=
# CFLAGS	= -Wall -Wextra -Werror


FT		= ./libft/
FT_LIB	= $(addprefix $(FT),libft.a)
FT_INC	= -I ./libft
FT_LNK	= -L ./libft -l ft

SRCDIR	= ./src/
INCDIR	= ./includes/
OBJDIR	= ./obj/

all: $(NAME)

$(OBJDIR)%.o:$(SRCDIR)%.c $(INCDIR) Makefile
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) $(FT_INC) -o $@ -c $< -I $(INCDIR)

$(NAME): $(OBJ) 
	make -C $(FT) 
	$(CC) $(OBJ) $(FT_LNK) -lm -o $(NAME)

clean:
	rm -rf $(OBJDIR)
	make -C $(FT) clean

fclean: clean
	rm -rf $(NAME)
	make -C $(FT) fclean

re: fclean all

.PHONY: all obj fclean clean re
