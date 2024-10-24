/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 23:08:12 by hel-asli          #+#    #+#             */
/*   Updated: 2024/10/24 07:27:10 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <fcntl.h>
# include <limits.h>
# include <signal.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <dirent.h>
# include <stdarg.h>
# include <termios.h>
# include <stdbool.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <sys/errno.h>
# include <sys/types.h>
# include <sys/ioctl.h>
# include <readline/history.h>
# include <readline/readline.h>

extern int	g_rl_signal;

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
}	t_syntax;

typedef enum e_red
{
	INPUT,
	OUT_TRUNC,
	OUT_APPEND,
	HEREDOC_INPUT,
}	t_red;

typedef struct s_env
{
	char				*key;
	struct s_env		*next;
	bool				exported;
	char				*value;
}	t_env;

typedef struct s_redirect
{
	t_red				type;
	char				*file;
	struct s_redirect	*next;
	bool				expanded;
	int					heredoc_fd;
	bool				is_ambgious;
}	t_redirect;

typedef struct s_commands
{
	t_env				*env;
	char				**args;
	t_redirect			*redirect;
	struct s_commands	*next;
}	t_commands;

typedef struct s_parsing
{
	char				*line;
}	t_parsing;

typedef struct s_exec
{
	char				**ev_execve;
	int					**fds;
	pid_t				*ids;
	pid_t				nbr;
}	t_exec;

typedef struct s_wildcard
{
	DIR					*dir;
	char				*prefix;
	char				*pwd;
	struct dirent		*entity;
}	t_wildcard;

typedef struct s_shell
{
	t_env				*env;
	t_exec				exec;
	t_parsing			parsing;
	t_commands			*commands;
	char				**ev_execve;
	struct termios		old_attr;
	struct termios		copy;
	int					exit_status;
	int					escape;
}	t_shell;

char					*ft_itoa(int n);
bool					is_valid(char c);
bool					is_space(char c);
bool					ft_isalpha(int c);
bool					ft_isdigit(int c);
bool					ft_isalnum(int c);
void					is_stat(char *cmd);
int						ft_atoi(char *str);
bool					is_special(char c);
size_t					arr_len(char **tab);
bool					is_ascii(char *str);
void					err_exit(char *str);
void					setup_signals(void);
void					set_env(t_env **env);
void					fr_args(char **args);
bool					in_quotes(char *str);
bool					check_var(char *arg);
char					*copy_key(char *src);
int						num_count(char *key);
bool					empty_str(char *line);
bool					is_builtin(char *str);
char					*del_quote(char *str);
void					err_handle(char *str);
void					env_clear(t_env **env);
char					**list_arr(t_env *env);
int						check_dots(char *line);
bool					check_file(char *file);
void					save_quotes(char *str);
void					gar_protect(char *str);
bool					is_rev_special(char c);
void					sigint_handler(int nb);
int						heredoc(t_shell *shell);
int						exec_pipe(t_exec *exec);
void					free_exec(t_exec *exec);
char					**ft_free(char **split);
void					protect_tab(char **tab);
void					sigquit_handler(int nb);
void					space_to_gar(char *line);
int						**fds_allocation(int nb);
void					setup_child_signal(void);
char					**ft_split(char const *s);
void					export_print(t_env **exp);
char					*check_value(char *value);
int						check_wildcard(char *str);
char					**ft_env_split(char *str);
t_commands				*ft_last(t_commands *node);
void					fds_free(int **fd, int nb);
size_t					ft_strlen(const char *str);
bool					is_valid_export(char *str);
int						ft_lstsize(t_commands *lst);
char					*ft_strdup(const char *str);
void					sort_export(t_env **export);
bool					is_redirection(char *token);
void					setup_heredoc_signals(void);
char					**wildcard_helper(char *arg);
bool					ft_strchr(char *str, char c);
bool					ft_lookup(char *str, char c);
void					match_rev(char *line, int i);
void					cmds_clear(t_commands **cmds);
void					match_char(char *line, int i);
char					*ft_strtok(char *str, char c);
void					ft_putstr_fd(char *s, int fd);
int						signal_status(t_shell *shell);
char					*my_strchr_v2(char *s, int c);
void					ft_putendl_fd(char *s, int fd);
t_env					*export_lstlast(t_env *export);
t_syntax				other_syntax_check(char *line);
void					sigint_heredoc_handler(int nb);
char					*get_env(char *key, t_env *env);
bool					ft_strstr(char *str, char *del);
void					exec_close(int **fds, int size);
char					*ft_strjoin(char *s1, char *s2);
void					syntax_err_msg(t_syntax syntax);
int						builtin_execute(t_shell *shell);
void					execution_start(t_shell *shell);
t_redirect				*build_redirection(char **args);
bool					quotes_syntax_check(char *line);
void					wild_init(t_wildcard *wildcard);
bool					is_exists(char *key, t_env *env);
bool					check_pipe(char **tokens, int i);
char					*str_add_char(char *str, char c);
t_redirect				*ft_last_redir(t_redirect *node);
void					built_env(t_env **env, char **ev);
char					**add_arr(char **args, char *str);
char					*ft_strcpy(char *dest, char *src);
char					*ft_strcat(char *dest, char *src);
void					wild_destory(t_wildcard *wildcard);
int						starts_with(char *start, char *str);
bool					check_heredoc(char **tokens, int i);
bool					wild_conditon(char *arg, char *pwd);
void					exit_error(t_shell *shell, int flag);
char					**ft_split_v2(const char *s, char c);
char					*find_command(char *cmd, t_env *env);
char					**get_files(char *str, t_wildcard *w);
t_redirect				*ft_new_redir(char *type, char *file);
bool					env_key_exist(t_env **env, char *key);
void					clear_redirect(t_redirect **redirect);
int						out_redirection(t_redirect *redirect);
char					*non_free_strjoin(char *s1, char *s2);
char					**wildcard_expand(char **args, int i);
char					*expand_arg(char *arg, t_shell *shell);
char					**re_build_arg(char **args, char **sp);
char					*ft_strndup(const char *str, int index);
long long				ft_exit_atol(t_shell *shell, char *str);
void					ft_lstadd_back(t_env **lst, t_env *new);
int						input_redirection(t_redirect *redirect);
bool					check_redirection(char **tokens, int i);
int						ft_strcmp(const char *s1, const char *s2);
t_commands				*ft_newlist(char **args, t_redirect *red);
void					signal_helper(t_shell *shell, int status);
int						handle_redirections(t_redirect *redirect);
char					**expand_args(char **args, t_shell *shell);
char					*heredoc_expand(char *str, t_shell *shell);
int						ft_fprintf(int fd, const char *format, ...);
size_t					count_non_redirection_arg_size(char **args);
void					ft_exit(t_exec *exec, t_shell *shell, int i);
bool					is_not_sub(const char *str, const char *pwd);
char					*ft_strjoin_char(char *s1, char *s2, char c);
bool					update_pwd(t_env **env, t_env *tmp, char *pwd);
bool					cd_home(t_env **env, char *oldpwd, int *status);
void					env_concat(t_env **env, char *key, char *value);
void					env_update(t_env **env, char *key, char *value);
char					*wildcard_dir(struct dirent *entity, char *str);
char					**args_allocation(char **tab, size_t arg_count);
void					set_terminal_new_attr(struct termios *old_attr);
t_env					*ft_lstnew(char *key, char *value, int exported);
void					read_input(t_shell *parsing, const char *prompt);
char					*get_from_env(t_shell *shell, char *arg, int *i);
void					process_pipe_cmds(t_shell **shell, char **pipes);
bool					my_env(t_commands *cmnds, t_shell *env, int flag);
char					*get_new_value(t_shell *shell, char *arg, int *i);
void					ft_back_addlst(t_commands **lst, t_commands *new);
void					ambigious_check(t_redirect *redirect, char	*file);
bool					my_pwd(t_commands *cmnds, t_shell *shell, int flag);
bool					check_pattern(char *pattern, char *str);
void					ft_lst_add_redir(t_redirect **lst, t_redirect *new);
void					restore_terminal_old_attr(struct termios *old_attr);
bool					my_exit(t_commands *cmnds, t_shell *shell, int flag);
bool					my_echo(t_commands *cmnds, t_shell *shell, int flag);
int						ft_strncmp(const char *s1, const char *s2, size_t n);
size_t					ft_strlcpy(char *dest, const char *src, size_t size);
bool					update_oldpwd(t_env **env, t_env *tmp, char *oldpwd);
char					**replace_tab(char **tab, char *arg, t_shell *shell);
void					expand_redirect(t_redirect *redirect, t_shell *shell);
void					wildcard_redirection(char *file, t_redirect *redirect);
void					file_change(t_redirect *tmp, char *file);
char					**wildcard_expand_helper(char **tab, char **args,
							int i);
bool					builtins_check(t_shell *shell, t_commands *cmnds,
							t_env **env, int flag);
void					env_export(t_env **env, char *key, char *value,
							int exported);
bool					cd_path(t_env **env, char *path, char *oldpwd,
							int *status);
char					*ft_substr(char const *s, unsigned int start,
							size_t len);
bool					my_export(t_commands *cmnds, t_shell *shell,
							t_env **env, int flag);
bool					my_unset(t_commands *cmnds, t_shell *shell,
							t_env **env, int flag);
void					execute_command_helper(t_commands *cmnds,
							t_shell *shell, t_exec *exec, int i);
bool					my_cd(t_commands *cmnds, t_shell *shell,
							t_env **env, int flag);
void					heredoc_helper(char *delimter, int fd,
							bool expanded, t_shell *shell);
char					*wildcard_file(struct dirent *entity,
							char *str, char *prefix);

#endif