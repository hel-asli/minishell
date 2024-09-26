/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-feng <oel-feng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 23:08:12 by hel-asli          #+#    #+#             */
/*   Updated: 2024/09/26 02:48:37 by oel-feng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <limits.h>
# include <stdio.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <sys/wait.h>
# include <stdbool.h>
# include <stdlib.h>
# include <sys/errno.h>
# include <unistd.h>
# include <fcntl.h>
# include <string.h>
# include <stdarg.h>


# define RED "\033[0;31m"
# define SYNTAX_QUOTES "minishell: Unclosed quotes"
# define SYNTAX_PIPE "minishell: Invalid pipe usage"
# define SYNTAX_REDIRECTION "minishell: Invalid redirection"
# define SYNTAX_HEREDOC "minishell: Invalid here-document (<<)"
# define SYNTAX_INVALID_APPEND "minishell: Invalid append redirection (>>)"

typedef enum e_syntax
{
	SYNTAX_OK,
	INVALID_HEREDOC,
	INVALID_REDIRECTINO,
	UNCLOSED_QUOTES,
	INVALIDE_PIPE,
	INVALID_APPEND,
}						t_syntax;

typedef enum e_red
{
	INPUT,
	HEREDOC_INPUT,
	OUT_APPEND,
	OUT_TRUNC,
} t_red;

typedef struct s_env
{
	char				*key;
	char				*value;
	bool				equal;
	struct s_env		*next;
}						t_env;


typedef struct s_redirect
{
	t_red				type;
	char				*file;
	int					heredoc_fd;
	bool				is_ambgious;
	bool				expanded;
	struct s_redirect	*next;
}						t_redirect;

typedef struct s_commands
{
	char				*cmd;
	char				**args;
	t_redirect			*redirect;

	t_env				*env;
	struct s_commands	*next;
}						t_commands;

typedef struct s_parsing
{
	char				*line;
}						t_parsing;

typedef struct s_shell
{
	t_env				*env;
	t_commands			*commands;
	t_parsing			parsing;
	t_env				*export;
	char				**ev_execve;
	int					exit_status;
}						t_shell;

//utils
char					*ft_itoa(int n);
bool					is_space(char c);
bool					ft_isalpha(int c);
bool					ft_isdigit(int c);
bool					ft_isalnum(int c);
int						ft_atoi(char *str);
bool					empty_str(char *line);
void					err_handle(char *str);
void					err_exit(char *str);
void					env_clear(t_env **env);
char					**ft_free(char **split);
long long				ft_exit_atol(char *str);
char					**ft_env_split(char *str);
char					**ft_split(char const *s);
size_t					ft_strlen(const char *str);
char					*ft_strdup(const char *str);
bool					ft_strchr(char *str, char c);
char					*my_strchr_v2(char *s, int c);
char					*ft_strtok(char *str, char c);
void					ft_putstr_fd(char *s, int fd);
void					ft_putendl_fd(char *s, int fd);
char					*get_env(char *key, t_env *env);
char					*ft_strjoin(char *s1, char *s2);
bool					ft_strstr(char *str, char *del);
char					**ft_split_v2(const char *s, char c);
char					*non_free_strjoin(char *s1, char *s2);
void					ft_lstadd_back(t_env **lst, t_env *new);
char					*ft_strndup(const char *str, int index);
int						ft_strcmp(const char *s1, const char *s2);
int						ft_fprintf(int fd, const char *format, ...);
t_env					*ft_lstnew(char *key, char *value, bool equal);
size_t					ft_strlcpy(char *dest, const char *src, size_t size);
int						ft_strncmp(const char *s1, const char *s2, size_t n);
char					*ft_substr(char const *s, unsigned int start, size_t len);
t_redirect				*ft_new_redir_v2(t_red type, char *file, bool expanded);
bool					in_quotes(char *str);
bool					is_rev_special(char c);
void					gar_protect(char *str);
bool					ft_is_ascii(char c);

//parsing
void					space_to_gar(char *line);
t_commands				*ft_last(t_commands *node);
bool					is_redirection(char *token);
t_syntax				other_syntax_check(char *line);
bool					quotes_syntax_check(char *line);
void					syntax_err_msg(t_syntax syntax);
t_redirect				*ft_last_redir(t_redirect *node);
bool					check_pipe(char **tokens, int i);
void					print_cmds(t_commands *commands);
bool					check_heredoc(char **tokens, int i);
t_redirect				*ft_new_redir(char *type, char *file);
bool					check_redirection(char **tokens, int i);
void					process_pipe_cmds(t_shell **shell, char **pipes);
void					ft_back_addlst(t_commands **lst, t_commands *new);
void					ft_lst_add_redir(t_redirect **lst, t_redirect *new);
char					*read_input(t_shell *parsing, const char *prompt, char **ev);
t_commands				*ft_newlist(char *cmd, char **args, t_redirect *red);
bool					is_ascii(char *str);
char					*del_quote(char *str);

// execution
bool					my_pwd(void);
bool					my_env(t_env **env);
void					exit_error(int flag);
bool					my_exit(t_commands **cmnds);
bool					my_echo(t_commands **cmnds);
t_env					*export_lstlast(t_env *export);
bool					my_cd(t_commands **cmnds, t_env **env);
void					export_env(t_env **env, t_env **export);
void					build_export(t_env **export, char **ev);
void					execution_start(t_shell *shell, char **ev);
void					env_update(t_env **env, char *key, char *value);
int 					execute(t_commands **cmnds, char **ev, int *tmp);
bool					my_unset(t_commands **cmnds, t_env **env, t_env **export);
bool				    my_export(t_commands **cmnds, t_env **env, t_env **export);
bool					builtins_check(t_commands **cmnds, t_env **env, t_env **export);
char					*expand_arg(char *arg, t_env *env, t_shell *shell);
char					*get_env(char *key, t_env *env);
void					save_quotes(char *str);
int 					handle_redirections(t_redirect *redirect);

#endif