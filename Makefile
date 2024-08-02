# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/30 23:25:40 by hel-asli          #+#    #+#              #
#    Updated: 2024/08/02 02:37:14 by hel-asli         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -g -fsanitize=address
SRCS = main.c utils/ft_split.c  utils/ft_itoa.c utils/ft_strncmp.c \
		utils/ft_putendl_fd.c utils/ft_strnstr.c utils/ft_strlen.c
OBJS = $(SRCS:.c=.o)
HEADER = minishell.h

all : $(NAME)

$(NAME) : $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@ -lreadline

%.o : %.c $(HEADER)
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

clean : 
	$(RM) $(OBJS)

fclean : clean
	$(RM) $(NAME)

re : fclean all

.PHONY : clean
