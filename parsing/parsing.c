/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 03:48:06 by hel-asli          #+#    #+#             */
/*   Updated: 2024/10/23 01:55:19 by hel-asli         ###   ########.fr       */
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

void	space_to_gar(char *line)
{
	int		i;
	bool	in_quotes;
	char	quote_type;

	(1) && (i = -1, in_quotes = false, quote_type = 0);
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

int	parse_input(t_shell *shell)
{
	int			exit;
	char		**pipes;
	t_syntax	syntax;

	exit = check_dots(shell->parsing.line);
	if (exit != 0)
		return (free(shell->parsing.line), (shell->exit_status = exit), 1);
	space_to_gar(shell->parsing.line);
	shell->parsing.line = add_spaces(shell->parsing.line);
	if (!shell->parsing.line)
		return (-1);
	if (!quotes_syntax_check(shell->parsing.line))
		syntax = UNCLOSED_QUOTES;
	else
		syntax = other_syntax_check(shell->parsing.line);
	if (syntax != SYNTAX_OK)
		return (syntax_err_msg(syntax), free(shell->parsing.line),
			shell->exit_status = 258, 1);
	pipes = ft_split_v2(shell->parsing.line, 124);
	free(shell->parsing.line);
	process_pipe_cmds(&shell, pipes);
	if (heredoc(shell))
		return (cmds_clear(&shell->commands), 1);
	return (0);
}

int signal_status(t_shell *shell)
{
	if (g_rl_signal == 3)
	{
		shell->exit_status = 1;
		g_rl_signal = 1;
	}
	if (!shell->parsing.line)
	{
		cmds_clear(&shell->commands);
		printf("exit\n");
		return (0);
	}
	return (1);
}

void	read_input(t_shell *shell, const char *prompt)
{
	while (true)
	{
		(1) && (g_rl_signal = 1, shell->commands = NULL);
		shell->parsing.line = readline(prompt);
		if (!signal_status(shell))
			break ;
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
