
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 23:08:12 by hel-asli          #+#    #+#             */
/*   Updated: 2024/08/05 05:50:50 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

typedef struct s_env
{
	char *key;
	char *value;
	struct s_env *next;
} t_env;

typedef enum e_syntax
{
	SYNTAX_OK,
	INVALID_HEREDOC,
	INVALID_REDIRECTINO,
	UNCLOSED_QUOTES,
	INVALIDE_PIPE,
	INVALID_APPEND,
}	t_syntax;

typedef struct s_commands
{
	char *cmd;
	char **args;
	struct s_commands *next;
} t_commands; 

typedef struct s_parsing
{
	char *line;
	t_commands *cmnds;
} t_parsing;


# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <limits.h>
# include <sys/errno.h>
# include <stdbool.h>
# include <readline/readline.h>
# include <readline/history.h>

// void parse_input(char *line);

//// utils 
char	**ft_split(char const *s);
char	**ft_free(char **split);
int		ft_atoi(const char *str);
size_t	ft_strlen(const char *str);
char	*ft_strdup(const char *str);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
int ft_strcmp(const char *s1, const char *s2);
void	ft_putstr_fd(char *s, int fd);
void	ft_putendl_fd(char *s, int fd);
void	err_handle(char *str);
bool	is_space(char c);
bool	empty_str(char *line);
bool	ft_strstr(char *str, char *del);
char	**ft_env_split(char *str);
void	ft_lstadd_back(t_env **lst, t_env *new);
t_env	*ft_lstnew(char *key, char *value);
void	env_clear(t_env **env);
void	set_env(t_env **env);

// parsing

#define SYNTAX_REDIRECTION "minishell: Invalid redirection"
#define SYNTAX_QUOTES "minishell: Unclosed quotes"
#define SYNTAX_PIPE "minishell: Invalid pipe usage"
#define SYNTAX_INVALID_APPEND "minishell: Invalid append redirection (>>)"
#define SYNTAX_HEREDOC "minishell: Invalid here-document (<<)"

char		*read_input(t_parsing *parsing,const char *prompt);
t_commands *ft_newlist(char *cmd, char **args);
size_t		ft_strlen(const char *str);
void		err_handle(char *str);
bool		is_space(char c);
bool		empty_str(char *line);
char		**ft_split(char const *s);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
int			ft_strcmp(const char *s1, const char *s2);
void	ft_back_addlst(t_commands **lst, t_commands *new);
t_commands	*ft_last(t_commands *node);
t_syntax	other_syntax_check(char *line);
char		**ft_free(char **split);
char	**ft_split_v2(const char *s, char c);
void		ft_putstr_fd(char *s, int fd);
bool		ft_strstr(char *str, char *del);
void		ft_putendl_fd(char *s, int fd);

#endif