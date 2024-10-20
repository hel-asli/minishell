/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-feng <oel-feng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 23:08:12 by hel-asli          #+#    #+#             */
/*   Updated: 2024/10/21 00:15:44 by oel-feng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <limits.h>
# include <stdio.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <termios.h>
# include <sys/wait.h>
# include <stdbool.h>
# include <stdlib.h>
# include <sys/errno.h>
# include <unistd.h>
# include <fcntl.h>
# include <string.h>
# include <sys/types.h>
# include <dirent.h>
# include <stdarg.h>
# include <sys/ioctl.h>

extern int rl_signal;

# define ANSI_CURSOR_UP "\033[1A"
# define ANSI_CURSOR_DOWN "\033[1B"
# define ANSI_ERASE_LINE "\033[2K"
# define SYNTAX_QUOTES "minishell: Unclosed quotes"
# define SYNTAX_PIPE "minishell: Invalid pipe usage"
# define SYNTAX_REDIRECTION "minishell: Invalid redirection"
# define SYNTAX_HEREDOC "minishell: Invalid here-document (<<)"
# define SYNTAX_INVALID_APPEND "minishell: Invalid append redirection (>>)"

typedef enum e_syntax
{
	SYNTAX_OK,
	INVALIDE_PIPE,
	INVALID_APPEND,
	UNCLOSED_QUOTES,
	INVALID_HEREDOC,
	INVALID_REDIRECTINO,
}						t_syntax;

typedef enum e_red
{
	INPUT,
	OUT_TRUNC,
	OUT_APPEND,
	HEREDOC_INPUT,
} t_red;

typedef struct s_env
{
	char				*key;
	struct s_env		*next;
	bool				exported;
	char				*value;
}						t_env;


typedef struct s_redirect
{
	t_red				type;
	char				*file;
	struct s_redirect	*next;
	bool				expanded;
	int					heredoc_fd;
	bool				is_ambgious;
}						t_redirect;

typedef struct s_commands
{
	t_env				*env;
	char				**args;
	t_redirect			*redirect;
	struct s_commands	*next;
}						t_commands;

typedef struct s_parsing
{
	char				*line;
}						t_parsing;

typedef struct s_exec
{
	char				**ev_execve;
	int					**fds;
	pid_t				*ids;
	pid_t				nbr;
}						t_exec;

typedef struct s_shell
{
	t_env				*env;
	t_exec				exec;
	t_parsing			parsing;
	t_commands			*commands;
	char				**ev_execve;
	struct				termios old_attr;
	struct				termios copy;
	int					exit_status;
	int					escape;
}						t_shell;

//utils
char					*ft_itoa(int n);
bool					is_space(char c);
void					fr_args(char **args);
bool					ft_isalpha(int c);
bool					ft_isdigit(int c);
bool					ft_isalnum(int c);
int						ft_atoi(char *str);
void					err_exit(char *str);
bool					in_quotes(char *str);
bool					empty_str(char *line);
void					err_handle(char *str);
char 					**list_arr(t_env *env);
bool					is_rev_special(char c);
void					gar_protect(char *str);
void					env_clear(t_env **env);
char					**ft_free(char **split);
long long				ft_exit_atol(char *str);
char					**ft_env_split(char *str);
char					**ft_split(char const *s);
size_t					ft_strlen(const char *str);
char					*ft_strdup(const char *str);
int						ft_lstsize(t_commands *lst);
bool					ft_strchr(char *str, char c);
bool					ft_lookup(char *str, char c);
char					*my_strchr_v2(char *s, int c);
char					*ft_strtok(char *str, char c);
void					ft_putstr_fd(char *s, int fd);
void					ft_putendl_fd(char *s, int fd);
char					*get_env(char *key, t_env *env);
char					*ft_strjoin(char *s1, char *s2);
bool					ft_strstr(char *str, char *del);
char					*ft_strcpy(char *dest, char *src);
char					*ft_strcat(char *dest, char *src);
t_env					*ft_lstnew(char *key, char *value, int exported);
char					**ft_split_v2(const char *s, char c);
char					*non_free_strjoin(char *s1, char *s2);
void					ft_lstadd_back(t_env **lst, t_env *new);
char					*ft_strndup(const char *str, int index);
int						ft_strcmp(const char *s1, const char *s2);
int						ft_fprintf(int fd, const char *format, ...);
char					*ft_strjoin_char(char *s1, char *s2, char c);
int						ft_strncmp(const char *s1, const char *s2, size_t n);
size_t					ft_strlcpy(char *dest, const char *src, size_t size);
void					free_exec(t_exec *exec);
bool					is_special(char c);
bool					is_rev_special(char c);
bool					in_quotes(char *str);
bool					ft_strchr(char *str, char c);
bool					check_file(char *file);
void					set_env(t_env **env);
void					match_rev(char *line, int i);
void					match_char(char *line, int i);
bool					is_valid(char c);
bool					check_var(char *arg);
void					built_env(t_env **env, char **ev);
char					*ft_substr(char const *s, unsigned int start,
							size_t len);
size_t					count_non_redirection_arg_size(char **args);
size_t					arr_len(char **tab);
char					*get_env(char *key, t_env *env);
char					*str_add_char(char *str, char c);
bool					is_valid_export(char *str);
void					fr_args(char **args);
char					**add_arr(char **args, char *str);
void					save_quotes(char *str);
void					gar_protect(char *str);
void					protect_tab(char **tab);
t_redirect				*build_redirection(char **args);
char					**args_allocation(char **tab, size_t arg_count);
char					*get_from_env(t_shell *shell, char *arg, int *i);
char					*get_new_value(t_shell *shell, char *arg, int *i);
char					**replace_tab(char **tab, char *arg, t_shell *shell);
char					**re_build_arg(char **args, char **sp);

//parsing
void					setup_signals(void);
bool					is_ascii(char *str);
void					sigint_heredoc_handler(int nb);
void					setup_heredoc_signals(void);
int						heredoc(t_shell *shell);
void					heredoc_helper(char *delimter, int fd, bool expanded, t_shell *shell);
char					*del_quote(char *str);
void					sigint_handler(int nb);
void					sigquit_handler(int nb);
void					space_to_gar(char *line);
t_commands				*ft_last(t_commands *node);
bool					is_redirection(char *token);
void					cmds_clear(t_commands **cmds);
t_syntax				other_syntax_check(char *line);
bool					quotes_syntax_check(char *line);
void					syntax_err_msg(t_syntax syntax);
t_redirect				*ft_last_redir(t_redirect *node);
bool					check_pipe(char **tokens, int i);
void					print_cmds(t_commands *commands);
bool					check_heredoc(char **tokens, int i);
void					clear_redirect(t_redirect **redirect);
t_redirect				*ft_new_redir(char *type, char *file);
bool					check_redirection(char **tokens, int i);
t_commands				*ft_newlist(char **args, t_redirect *red);
void					set_terminal_new_attr(struct termios *old_attr);
void					process_pipe_cmds(t_shell **shell, char **pipes);
void					read_input(t_shell *parsing, const char *prompt);
void					ft_back_addlst(t_commands **lst, t_commands *new);
void					ft_lst_add_redir(t_redirect **lst, t_redirect *new);
void					restore_terminal_old_attr(struct termios *old_attr);

// execution
bool					my_pwd(t_commands *cmnds, t_shell *shell, int flag);
bool					my_env(t_commands *cmnds,  t_shell *env, int flag);
void					exit_error(int flag);
void					save_quotes(char *str);
bool					my_exit(t_commands *cmnds, t_shell *shell, int flag);
bool					my_echo(t_commands *cmnds, t_shell *shell, int flag);
t_env					*export_lstlast(t_env *export);
void					execution_start(t_shell *shell);
char					*get_env(char *key, t_env *env);
// void					export_env(t_env **env, char *args);
char 					*find_command(char *cmd, t_env *env);
bool					my_cd(t_commands *cmnds, t_shell *shell, t_env **env, int flag);
// void					build_export(t_env **export, char **ev);
bool					my_unset(t_commands *cmnds, t_shell *shell, t_env **env, int flag);
bool				    my_export(t_commands *cmnds, t_shell *shell, t_env **env, int flag);
bool					builtins_check(t_shell *shell, t_commands *cmnds, t_env **env, int flag);
void					env_update(t_env **env, char *key, char *value);
int						handle_redirections(t_redirect *redirect);
char					*expand_arg(char *arg, t_shell *shell);
char					**list_arr(t_env *env);

#endif