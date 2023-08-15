# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: oel-ayad <oel-ayad@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/01/30 15:43:53 by oel-ayad          #+#    #+#              #
#    Updated: 2023/08/15 17:33:14 by oel-ayad         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

SRCS =	ft_memset.c \
		ft_bzero.c \
		ft_atoi.c \
		ft_atol.c \
		ft_putchar.c \
		ft_putstr.c \
		ft_putendl.c \
		ft_putchar_fd.c \
		ft_putstr_fd.c \
		ft_putendl_fd.c \
		ft_isalnum.c \
		ft_strndup.c \
		ft_strlen.c \
		ft_toupper.c \
		ft_tolower.c \
		ft_isupper.c \
		ft_utoa_base.c \
		ft_strcmp.c \
		ft_memcpy.c \
		ft_strncmp.c \
		ft_strnew.c \
		ft_strdup.c \
		ft_strlowcase.c \
		ft_isspace.c \
		ft_isalpha.c \
		ft_memalloc.c \
		ft_isdigit.c \
		ft_isalnum.c \

OBJ = $(SRCS:.c=.o)

CFLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OBJ)
	cc $(CFLAGS) -c $(SRCS)
	ar rc $(NAME) $(OBJ)
	ranlib $(NAME)

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all obj fclean clean re
