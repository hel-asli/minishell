# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/30 23:25:40 by hel-asli          #+#    #+#              #
#    Updated: 2024/10/20 06:31:41 by hel-asli         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = cc
CFLAGS = #-Wall -Wextra -Werror
# INCLUDE_DIR = Include
LDFLAGS = # -g -fsanitize=address
READLINE_PREFIX = $(shell brew --prefix readline)
READLINE_INCLUDE = $(READLINE_PREFIX)/include
READLINE_LIB = $(READLINE_PREFIX)/lib
LIBS = -L$(READLINE_LIB) -lreadline
INCLUDES = -I$(READLINE_INCLUDE)

SRCS = main.c utils/ft_split.c utils/ft_itoa.c utils/ft_strncmp.c \
		utils/ft_putendl_fd.c utils/ft_strstr.c utils/ft_strlen.c \
		utils/ft_atoi.c utils/error.c utils/ft_lst.c utils/ft_env_split.c \
		parsing/parsing.c utils/ft_split_v2.c parsing/syntax_helper.c \
		utils/ft_redir_lst.c utils/ft_substr.c parsing/syntax_check.c \
		parsing/parsing_helper.c utils/ft_strdup.c utils/ft_strjoin.c \
		execution/execution.c builtins/env.c builtins/pwd.c builtins/echo.c \
		builtins/unset.c builtins/exit.c builtins/export.c builtins/cd.c \
		utils/ascii_check.c utils/ft_strtok.c utils/ft_strchr.c execution/exec.c \
		utils/ft_fprintf.c utils/ft_lst_2.c utils/env.utils.c utils/parsing_utils.c \
		utils/checks.c parsing/signals.c parsing/heredoc.c utils/expand_utils.c \
		utils/expand_utils_2.c utils/expand_utils_3.c

HEADER = minishell.h
OBJS = $(SRCS:.c=.o)

all : $(NAME)

$(NAME) : $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@ $(LIBS)

%.o : %.c  $(HEADER)
	$(CC) $(CFLAGS) $(LDFLAGS) $(INCLUDES) -c $< -o $@

clean :
	$(RM) $(OBJS) $(UTILS_OBJS) $(PARS_OBJS)

fclean : clean
	$(RM) $(NAME)

re : fclean all

.PHONY : clean

