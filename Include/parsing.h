/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 03:51:16 by hel-asli          #+#    #+#             */
/*   Updated: 2024/08/04 07:12:58 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include <readline/readline.h>
# include <readline/history.h>
# include <stdbool.h>
# include <stdlib.h>
# include <unistd.h>

typedef enum e_syntax
{
	SYNTAX_OK,
	INVALID_HEREDOC,
	INVALID_REDIRECTINO,
	UNCLOSED_QUOTES,
	INVALIDE_PIPE,
	INVALID_APPEND,
}	t_syntax;


#define SYNTAX_REDIRECTION "Error: Invalid redirection"
#define SYNTAX_QUOTES "Error: Unclosed quotes"
#define SYNTAX_PIPE "Error: Invalid pipe usage"
#define SYNTAX_INVALID_APPEND "Error: Invalid append redirection (>>)"
#define SYNTAX_HEREDOC "Error: Invalid here-document (<<)"

char		*read_input(const char *prompt);
size_t		ft_strlen(const char *str);
void		err_handle(char *str);
bool		is_space(char c);
bool		empty_str(char *line);
char		**ft_split(char const *s);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
int			ft_strcmp(const char *s1, const char *s2);
t_syntax	other_syntax_check(char *line);
char		**ft_free(char **split);
void		ft_putstr_fd(char *s, int fd);
bool		ft_strstr(char *str, char *del);
void		ft_putendl_fd(char *s, int fd);

# endif