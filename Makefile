# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/30 23:25:40 by hel-asli          #+#    #+#              #
#    Updated: 2024/08/03 04:04:12 by hel-asli         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror
INCLUDE_DIR = Include
LDFLAGS = -g -fsanitize=address
UTILS_SRCS = utils/ft_split.c  utils/ft_itoa.c utils/ft_strncmp.c \
			utils/ft_putendl_fd.c utils/ft_strstr.c utils/ft_strlen.c \
			utils/ft_atoi.c utils/error.c
PARS_SRCS = parsing/parsing.c
SRCS = main.c
UTILS_HEADER = Include/utils.h
PARS_HEADER = Include/parsing.h
HEADER = Include/minishell.h
UTILS_OBJS = $(UTILS_SRCS:.c=.o)
PARS_OBJS = $(PARS_SRCS:.c=.o);
OBJS = $(SRCS:.c=.o)

all : $(NAME)

$(NAME) : $(OBJS) $(UTILS_OBJS) $(PARS_OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@ -lreadline

utils/%.o : utils/%.c $(UTILS_HEADER)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) $(LDFLAGS) -c $< -o $@

parsing/%.o : parsing/%.c $(PARS_HEADER)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) $(LDFLAGS) -c $< -o $@

%.o : %.c  $(HEADER)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) $(LDFLAGS) -c $< -o $@

clean : 
	$(RM) $(OBJS) $(UTILS_OBJS) $(PARS_OBJS)

fclean : clean
	$(RM) $(NAME)

re : fclean all

.PHONY : clean
