/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 03:48:06 by hel-asli          #+#    #+#             */
/*   Updated: 2024/10/15 02:18:58 by hel-asli         ###   ########.fr       */
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
	char	*new_line ;

	(1) && (i = 0, j = 0, len = count_len(line));
	new_line = malloc(sizeof(char) * len + 1);
	if (!new_line)
		return (free(line), NULL);
	while (line[i] && j < len)
	{
		if (ft_strstr(&line[i], ">>") || ft_strstr(&line[i], "<<"))
		{
			(1) && (new_line[j++] = 32, new_line[j++] = line[i++]);
			(1) && (new_line[j++] = line[i++],new_line[j++] = 32);
		}
		else if (ft_strstr(&line[i], ">")
			|| ft_strstr(&line[i], "<") || ft_strstr(&line[i], "|"))
		{
			(1) && (new_line[j++] = 32, new_line[j++] = line[i++]);
			new_line[j++] = 32;
		}
		else
			new_line[j++] = line[i++];
	}
	new_line[j] = '\0';
	return (free(line), new_line);
}

bool is_special(char c)
{
	return (c == '|' || c == '<' || c == '>' || is_space(c));
}

bool is_rev_special(char c)
{
	return (c == 1 || c == 2 || c == 3 || c == 4 || (c >= 15 && c <= 19));
}

void match_char(char *line, int i)
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

void space_to_gar(char *line)
{
    int i = 0;
    bool in_quotes = false;
    char quote_type = 0;

    while (line[i])
    {
        if (line[i] == '"' || line[i] == '\'')
        {
            if (!in_quotes)
            {
                in_quotes = true;
                quote_type = line[i];
            }
            else if (line[i] == quote_type)
            {
                in_quotes = false;
            }
            else
            {
				if (is_special(line[i]))
					match_char(line, i);
            }
        }
        else if (in_quotes)
        {
			if (is_special(line[i]))
				match_char(line, i);
        }
        i++;
    }
}

char* del_quote(char *str)
{
    int     i;
    char *ptr;
    char    type = '\0';
    bool    in_quote;
    i = 0;
    int j = 0;

    in_quote = false;
    ptr = malloc(sizeof(char) * strlen(str) + 1);
    while (str[i])
    {
        if (!in_quote && (str[i] == '\'' || str[i] == '"'))
        {
            in_quote = !in_quote;
            type = str[i];
        }
        else if (in_quote && type == str[i])
            in_quote = false;
        else
            ptr[j++] = str[i];
        i++;
    }
    ptr[j] = '\0';
	free(str);
    return (ptr);
}

bool in_quotes(char *str)
{
	int i = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
			return (true);
		i++;
	}
	return (false);
}

int heredoc_helper(char *delimter, int fd, bool expanded, t_shell *shell)
{
	char *line;
	t_env  *env;

	line = NULL;
	env = shell->env;
	rl_signal = 2;
	while (true)
	{
			line = readline("> ");
			if (!line)
				break;
			if (rl_signal == 1)
			{
				close(fd);
				free(line);
				line = NULL;
				return (1);
			}
			
			if (!ft_strcmp(line, delimter))
				break ;
			if (expanded)	
			{
				line = expand_arg(line, env, shell);
				if (!line)
					line = ft_strdup("");
			}
			write(fd, line, ft_strlen(line));
			write(fd, "\n", 1);
	}
	rl_signal = 1;
	return (0);
}

int heredoc(t_shell *shell)
{
	t_commands *cmd = shell->commands;
	t_redirect *red = NULL;
	int heredoc_read;
	int heredoc_write;

	while (cmd)
	{
		red = cmd->redirect;
		while (red)
		{
			if (red->type == HEREDOC_INPUT)
			{
				int *nbr = malloc(sizeof(int));
				char *name = ft_strjoin(ft_strdup("/tmp/.heredoc"), ft_itoa((long)nbr));
				free(nbr);
				if (!name)
					err_handle("malloc");
				heredoc_write = open(name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
				heredoc_read = open(name, O_RDONLY, 0644);
				if (unlink(name) < 0)
					err_exit("unlink");
				free(name);
				if (heredoc_helper(red->file, heredoc_write, red->expanded, shell))
				{
					close(heredoc_read);
            		printf("%s%s", ANSI_CURSOR_UP, ANSI_ERASE_LINE);
					return 1;
				}
				close(heredoc_write);
				red->heredoc_fd = heredoc_read;
			}
			red = red->next;
		}
		cmd = cmd->next;
	}
	return (0);
}

int	parse_input(t_shell *shell)
{
	char		*new_line;
	char		**pipes;
	t_syntax	syntax;
	space_to_gar(shell->parsing.line); 
	new_line = add_spaces(shell->parsing.line);
	shell->parsing.line = new_line;
	if (!new_line)
		return (-1);
	if (!quotes_syntax_check(new_line))
		syntax = UNCLOSED_QUOTES;
	else
		syntax = other_syntax_check(new_line);
	if (syntax != SYNTAX_OK)
		return (syntax_err_msg(syntax), free(new_line), 1);
	pipes = ft_split_v2(shell->parsing.line, 124);
	process_pipe_cmds(&shell, pipes);
	if (heredoc(shell))
		return (-1);
	print_cmds(shell->commands);
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
			printf("%s%sminishell: exit\n", ANSI_CURSOR_UP, ANSI_ERASE_LINE);
			break ;
		}
		if (shell->parsing.line && *shell->parsing.line)
			add_history(shell->parsing.line);
		if (!ft_strlen(shell->parsing.line) || empty_str(shell->parsing.line))
		{
			free(shell->parsing.line);
			continue ;
		}
		if (parse_input(shell) == -1)
			continue ;
		restore_terminal_old_attr(&shell->old_attr);
		rl_signal = 0;
		execution_start(shell);
		restore_terminal_old_attr(&shell->copy);
		free(shell->parsing.line);
	}

}
