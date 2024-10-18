/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 03:48:06 by hel-asli          #+#    #+#             */
/*   Updated: 2024/10/18 11:15:48 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

size_t	count_len(char *line)
{
	int		i;
	size_t	len;

	i = 0;
	len = ft_strlen(line);
	while (line[i])
	{
		if (ft_strstr(&line[i], ">>") || ft_strstr(&line[i], "<<"))
		{
			len += 2;
			i += 2;
		}
		else if (ft_strstr(&line[i], ">")
			|| ft_strstr(&line[i], "<") || ft_strstr(&line[i], "|"))
		{
			len += 2;
			i++;
		}
		else
			i++;
	}
	return (len);
}

char	*add_spaces(char *line)
{
	size_t	j;
	size_t	i;
	size_t	len;
	char	*nl;

	(1) && (i = 0, j = 0, len = count_len(line));
	nl = malloc(sizeof(char) * len + 1);
	if (!nl)
		return (free(line), NULL);
	while (line[i] && j < len)
	{
		if (ft_strstr(&line[i], ">>") || ft_strstr(&line[i], "<<"))
		{
			(1) && (nl[j++] = 32, nl[j++] = line[i++]);
			(1) && (nl[j++] = line[i++], nl[j++] = 32);
		}
		else if (ft_strstr(&line[i], ">")
			|| ft_strstr(&line[i], "<") || ft_strstr(&line[i], "|"))
		{
			(1) && (nl[j++] = 32, nl[j++] = line[i++], nl[j++] = 32);
		}
		else
			nl[j++] = line[i++];
	}
	return (nl[j] = 0, free(line), nl);
}

bool	is_special(char c)
{
	return (c == '|' || c == '<' || c == '>' || is_space(c));
}

bool	is_rev_special(char c)
{
	return (c == 1 || c == 2 || c == 3 || c == 4 || (c >= 15 && c <= 19));
}

void	match_char(char *line, int i)
{
	if (line)
	{
		if (line[i] == '|')
			line[i] = 1;
		else if (line[i] == '<')
			line[i] = 2;
		else if (line[i] == '>')
			line[i] = 3;
		else if (line[i] == 32)
			line[i] = 4;
		else if (line[i] >= 9 && line[i] <= 13)
			line[i] += 6;
	}
}

void	space_to_gar(char *line)
{
	int		i;
	bool	in_quotes;
	char	quote_type;

	(1) && (i = -1 ,in_quotes = false, quote_type = 0);
	while (line[++i])
	{
		if (line[i] == '"' || line[i] == '\'')
		{
			if (!in_quotes)
				(1) && (in_quotes = true, quote_type = line[i]);
			else if (line[i] == quote_type)
				in_quotes = false;
			else if (is_special(line[i]))
				match_char(line, i);
		}
		else if (in_quotes && is_special(line[i]))
			match_char(line, i);
	}
}

char	*del_quote(char *str)
{
	int		i;
	int		j;
	char	*ptr;
	char	type;
	bool	in_quote;

	(1) && (j = 0, i = 0, in_quote = false, type = 0);
	ptr = malloc(sizeof(char) * ft_strlen(str) + 1);
	if (!ptr)
		err_handle("Malloc Failure");
	while (str[i])
	{
		if (!in_quote && (str[i] == '\'' || str[i] == '"'))
			(1) && (in_quote = !in_quote, type = str[i]);
		else if (in_quote && type == str[i])
			in_quote = false;
		else
			ptr[j++] = str[i];
		i++;
	}
	return (ptr[j] = 0, free(str), ptr);
}

bool	in_quotes(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
			return (true);
		i++;
	}
	return (false);
}

void	heredoc_helper(char *delimter, int fd, bool expanded, t_shell *shell)
{
	char	*line;
	t_env	*env;

	(1) && (line = NULL, env = shell->env);
	while (true)
	{
		line = readline("> ");
		if (!line)
		{
			free(line);
			break ;
		}
		if (!ft_strcmp(line, delimter))
			break ;
		if (expanded)	
		{
			line = expand_arg(line, env, shell, NULL);
			if (!line)
				line = ft_strdup("");
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
}

void	sigint_heredoc_handler(int nb)
{
	if (nb == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		exit(1);
	}
}
void	setup_heredoc_signals(void)
{
	signal(SIGINT, sigint_heredoc_handler);
	signal(SIGQUIT, SIG_IGN);
}

int		heredoc(t_shell *shell)
{
	t_commands *cmd = shell->commands;
	t_redirect *red = NULL;
	int heredoc_write;
	int status;

	rl_signal = 0;
	while (cmd)
	{
		red = cmd->redirect;
		while (red)
		{
			status = 0;
			if (red->type == HEREDOC_INPUT)
			{
				int *nbr = malloc(sizeof(int) * 1);
				char *name = ft_strjoin(ft_strdup("/tmp/.heredoc"), ft_itoa((long)nbr));
				free(nbr);
				if (!name)
					err_handle("malloc");
				heredoc_write = open(name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
				red->heredoc_fd = open(name, O_RDONLY, 0644);
				if (unlink(name) < 0)
					err_exit("unlink");
				free(name);
				pid_t id = fork();
				if (id == 0)
				{
					setup_heredoc_signals();
					heredoc_helper(red->file, heredoc_write, red->expanded, shell);
					close(heredoc_write);
					cmds_clear(&shell->commands);
					env_clear(&shell->env);
					// close(red->heredoc_fd);
					exit(0);
				}
				else if (id > 0)
				{
					waitpid(id, &status, 0);
					close(heredoc_write);
					if (WIFEXITED(status) && WEXITSTATUS(status))
					{
						close(red->heredoc_fd);
						close(heredoc_write);
						shell->exit_status = WEXITSTATUS(status);
						return (1);
					}
				}
			}
			red = red->next;
		}
		cmd = cmd->next;
	}
	return (0);
}

int	parse_input(t_shell *shell)
{
	char		**pipes;
	t_syntax	syntax;

	space_to_gar(shell->parsing.line); 
	shell->parsing.line = add_spaces(shell->parsing.line);
	if (!shell->parsing.line)
		return (-1);
	if (!quotes_syntax_check(shell->parsing.line))
		syntax = UNCLOSED_QUOTES;
	else
		syntax = other_syntax_check(shell->parsing.line);
	if (syntax != SYNTAX_OK)
		return (syntax_err_msg(syntax), free(shell->parsing.line), shell->exit_status = 258, 1);
	pipes = ft_split_v2(shell->parsing.line, 124);
	free(shell->parsing.line);
	process_pipe_cmds(&shell, pipes);
	if (heredoc(shell))
		return (1);
	return (0);
}

void	read_input(t_shell *shell, const char *prompt)
{
	while (true)
	{
		rl_signal = 1;
		shell->commands = NULL;
		shell->parsing.line = readline(prompt);
		if (!shell->parsing.line)
		{
			cmds_clear(&shell->commands);
			printf("exit\n");
			break ;
		}
		if (shell->parsing.line && *shell->parsing.line)
			add_history(shell->parsing.line);
		if (!ft_strlen(shell->parsing.line) || empty_str(shell->parsing.line))
		{
			free(shell->parsing.line);
			cmds_clear(&shell->commands);
			continue ;
		}
		if (parse_input(shell) || !shell->commands)
			continue ;
		restore_terminal_old_attr(&shell->old_attr);
		execution_start(shell); 
		cmds_clear(&shell->commands);
		restore_terminal_old_attr(&shell->copy);
	}
}
