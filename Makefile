# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/30 23:25:40 by hel-asli          #+#    #+#              #
#    Updated: 2024/09/25 05:51:26 by hel-asli         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror
INCLUDE_DIR = Include
LDFLAGS = -g -fsanitize=address

SRCS = main.c utils/ft_split.c utils/ft_itoa.c utils/ft_strncmp.c \
		utils/ft_putendl_fd.c utils/ft_strstr.c utils/ft_strlen.c \
		utils/ft_atoi.c utils/error.c utils/ft_lst.c utils/ft_env_split.c \
		parsing/parsing.c utils/ft_split_v2.c parsing/syntax_helper.c \
		utils/ft_redir_lst.c utils/ft_substr.c parsing/syntax_check.c \
		parsing/parsing_helper.c utils/ft_strdup.c utils/ft_strjoin.c \
		execution/execution.c builtins/env.c builtins/pwd.c builtins/echo.c \
		builtins/unset.c builtins/exit.c builtins/export.c builtins/cd.c \
		utils/ascii_check.c utils/ft_strtok.c utils/ft_strchr.c execution/exec.c \
		utils/ft_fprintf.c \

HEADER = minishell.h
OBJS = $(SRCS:.c=.o)

all : $(NAME)

$(NAME) : $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@ -lreadline

%.o : %.c  $(HEADER)
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

clean :
	$(RM) $(OBJS) $(UTILS_OBJS) $(PARS_OBJS)

fclean : clean
	$(RM) $(NAME)

re : fclean all

.PHONY : clean

