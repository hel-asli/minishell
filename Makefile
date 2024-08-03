# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/30 23:25:40 by hel-asli          #+#    #+#              #
#    Updated: 2024/08/03 00:40:23 by hel-asli         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror
INCLUDE_DIR = Include
LDFLAGS = -g -fsanitize=address
UTILS_SRCS = utils/ft_split.c  utils/ft_itoa.c utils/ft_strncmp.c \
			utils/ft_putendl_fd.c utils/ft_strnstr.c utils/ft_strlen.c \
			utils/ft_atoi.c
SRCS = main.c
UTILS_HEADER = Include/utils.h
HEADER = Include/minishell.h
UTILS_OBJS = $(UTILS_SRCS:.c=.o)
OBJS = $(SRCS:.c=.o)

all : $(NAME)

$(NAME) : $(OBJS) $(UTILS_OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@ -lreadline

utils/%.o : utils/%.c $(UTILS_HEADER)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) $(LDFLAGS) -c $< -o $@

%.o : %.c  $(HEADER)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) $(LDFLAGS) -c $< -o $@

clean : 
	$(RM) $(OBJS) $(UTILS_OBJS)

fclean : clean
	$(RM) $(NAME)

re : fclean all

.PHONY : clean
