/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-feng <oel-feng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 03:48:06 by hel-asli          #+#    #+#             */
/*   Updated: 2024/09/28 02:51:41 by oel-feng         ###   ########.fr       */
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

	i = 0;
	j = 0;
	len = count_len(line);
	new_line = malloc(sizeof(char) * len + 1);
	if (!new_line)
		return (free(line), NULL);
	while (line[i] && j < len)
	{
		if (ft_strstr(&line[i], ">>") || ft_strstr(&line[i], "<<"))
		{
			(new_line[j++] = 32, new_line[j++] = line[i++]);
			(new_line[j++] = line[i++],new_line[j++] = 32);
		}
		else if (ft_strstr(&line[i], ">")
			|| ft_strstr(&line[i], "<") || ft_strstr(&line[i], "|"))
		{
			new_line[j++] = 32;
			new_line[j++] = line[i++];
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

void del_quotes(t_shell *shell)
{
	t_commands *commands;
	t_redirect *red;
	int i = 0;

	commands = shell->commands;
	while (commands)
	{
		if (commands->cmd)
		{
			commands->cmd = del_quote(commands->cmd);
		}
		i = 0;
		while (commands->args[i])
		{
			commands->args[i] =  del_quote(commands->args[i]);
			i++;
		}
		red = commands->redirect;
		while (red)
		{
			if (red->type == HEREDOC_INPUT && !in_quotes(red->file))
				red->expanded = true;
			red->file = del_quote(red->file);
			red = red->next;
		}
		commands = commands->next;
	}
}

void heredoc_helper(char *delimter, int fd, bool expanded, t_shell *shell)
{
	char *line;
	t_env  *env;

	line = NULL;
	env = shell->env;
	while (true)
	{
			line = readline("> ");
			if (!line)
				break ;
			if (!ft_strcmp(line, delimter))
				break ;
			if (expanded)	
			{
				line = expand_arg(line, env, shell);
				if (!line)
					line = ft_strdup("\n");
			}
			printf("line : [%s]\n", line);
			write(fd, line, ft_strlen(line));
			write(fd, "\n", 1);
	}
}
void heredoc(t_shell *shell)
{
	t_commands *cmd = shell->commands;
	t_redirect *red = NULL;
	int heredoc_read;
	int heredoc_write;

	while (cmd)
	{
		int *nbr = malloc(sizeof(int));
		long nb = (long)nbr;
		char *name = ft_strjoin(ft_strdup("/tmp/.heredoc"), ft_itoa(nb));
		if (!name)
			err_handle("malloc");
		red = cmd->redirect;
		while (red)
		{
			heredoc_write = open(name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
			heredoc_read = open(name, O_RDONLY, 0644);
			if (unlink(name) < 0)
				err_exit("unlink");
			if (red->type == HEREDOC_INPUT)
				heredoc_helper(red->file, heredoc_write, red->expanded, shell);
			close(heredoc_write);
			red->heredoc_fd = heredoc_read;
			red = red->next;
		}
		cmd = cmd->next;
	}
}

int calc_heredoc(t_redirect *redirect)
{
	int i = 0;
	t_redirect *tmp;

	tmp = redirect;
	while (tmp)
	{
		if (tmp->type == HEREDOC_INPUT)
			i++;
		tmp = tmp->next;
	}

	return (i);
}

t_redirect *heredoc_del(t_redirect *red, int heredoc_lenght)
{
	t_redirect *new_red = NULL;
	int i  = heredoc_lenght;

	while (red)	
	{
		if (red->type == HEREDOC_INPUT && i > 1)
		{
			red = red->next;
			i--;
		}
		else
		{
			ft_lst_add_redir(&new_red, ft_new_redir_v2(red->type, red->file, red->expanded));
			red = red->next;
		}
	}
	return (new_red);
}

void redirection_helper(t_shell *shell)
{
	t_commands *cmds;
	t_redirect *red;
	int heredoc_length;
	t_redirect  *new_red;

	heredoc_length = 0;
	cmds = shell->commands;
	while (cmds)
	{
		red = cmds->redirect;
		if (red)
			heredoc_length = calc_heredoc(red);
		if (heredoc_length > 1)
		{
			new_red = heredoc_del(red, heredoc_length);
			// clear old_redirection. 
			cmds->redirect = new_red;
		}
		cmds = cmds->next;
	}
}
int	parse_input(t_shell *shell)
{
	char		*new_line;
	char		**pipes;
	t_syntax	syntax;
	// printf("-> %s\n", shell->parsing.line);
	space_to_gar(shell->parsing.line); // save space and | inside "" . 
	// printf("-> %s\n", shell->parsing.line);
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
	heredoc(shell);
	redirection_helper(shell);
	// print_cmds(shell->commands);
	return (0);
}

char	*read_input(t_shell *shell, const char *prompt, char **ev)
{
	while (true)
	{
		shell->parsing.line = readline(prompt);
		if (!shell->parsing.line)
			err_handle("exit");
		add_history(shell->parsing.line);
		if (!ft_strlen(shell->parsing.line) || empty_str(shell->parsing.line))
		{
			free(shell->parsing.line);
			continue ;
		}
		if (parse_input(shell) == 1)
			continue ;
		execution_start(shell, ev);
		free(shell->parsing.line);
	}
	return (shell->parsing.line);
}
