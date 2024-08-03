/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 03:48:06 by hel-asli          #+#    #+#             */
/*   Updated: 2024/08/03 23:49:39 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Include/parsing.h"

bool quotes_syntax_check(char *line)
{
	bool check = false;
	char	c = 0;
	int i = 0;

	while (line[i])
	{
		if (!check && (line[i] == 39 || line[i] == 34))
		{
			check = true;
			c = line[i];
		}
		else if (check && line[i] == c)
			check = false;
		i++;
	}
	return (!check);
}

bool long_heredoc(char *str)
{
	size_t i = 0;

	while (str[i] && str[i] == '<')
		i++;
	if (i >= 3 && i == ft_strlen(str))
		return (true);
	return (false);
}

bool long_append(char *str)
{
	size_t i = 0;

	while (str[i] && str[i] == '>')
		i++;
	if (i >= 3 && i == ft_strlen(str))
		return (true);
	return (false);
}


void print_splited(char **line)
{
	int i = 0;
	while (line[i])
	{
		printf("-- %s\n", line[i]);
		i++;
	}
}
t_syntax other_syntax_check(char *line)
{
	int i = 0;
	t_syntax result;
	char **sp = ft_split(line);
	print_splited(sp);
	if (!sp)
		err_handle("Allocation Fail");
	
	result = SYNTAX_OK;
	while (sp[i])
	{
		if (sp[i + 1] && !ft_strcmp(sp[i], ">>") && (!ft_strcmp(sp[i + 1], ">>") || !ft_strcmp(sp[i + 1], ">")))
			result = LONG_APPEND;
		else if (sp[i + 1] && !ft_strcmp(sp[i], "<<") && (!ft_strcmp(sp[i + 1], "<<") || !ft_strcmp(sp[i + 1], "<")))	
			result = LONG_HEREDOC;
		else if (!ft_strcmp(sp[i], ">>") || !ft_strcmp(sp[i], "<<") || !ft_strcmp(sp[i], ">") || !ft_strcmp(sp[i], "<"))
		{
			if (i > 0 && (sp[i - 1] == NULL || sp[i + 1] == NULL))
				result = INVALID_REDIRECTINO;
			else
				if (sp[i + 1] == NULL)
					result = INVALID_REDIRECTINO;
		}
		else if (!ft_strcmp(sp[i], "|"))
		{
			if (i == 0 || sp[i + 1] == NULL)
				result = INVALIDE_PIPE;
		}
		i++;
	}
	ft_free(sp);
	return (result);
}

void syntax_err_msg(t_syntax syntax)
{
	if (syntax == INVALID_REDIRECTINO)
		ft_putendl_fd(SYNTAX_REDIRECTION, STDERR_FILENO);
	else if (syntax == UNCLOSED_QUOTES)
		ft_putendl_fd(SYNTAX_QUOTES, STDERR_FILENO);
	else if (syntax == INVALIDE_PIPE)
		ft_putendl_fd(SYNTAX_PIPE, STDERR_FILENO);
	else if (syntax == LONG_APPEND)
		ft_putendl_fd(SYNTAX_LONG_APPEND, STDERR_FILENO);
	else if (syntax == LONG_HEREDOC)
		ft_putendl_fd(SYNTAX_LONG_HEREDOC, STDERR_FILENO);
}
size_t count_len(char *line)
{
	size_t len = ft_strlen(line);
	int i = 0;

	while (line[i])
	{
		if (ft_strstr(&line[i], ">>") || ft_strstr(&line[i], "<<"))
		{
			len += 2;
			i += 2;
		}
		else if (ft_strstr(&line[i], ">") || ft_strstr(&line[i], "<") || ft_strstr(&line[i], "|"))
		{
			len += 2;
			i++;
		}
		else
			i++;
	}

	return (len);
}

char *add_spaces(char *line)
{
	size_t len = count_len(line);
	char *new_line = malloc(sizeof(char) * len + 1);
	size_t j = 0;
	size_t i = 0;
	if (!new_line)
		return (NULL);
	while (line[i] && j < len)
	{
		if (ft_strstr(&line[i], ">>") || ft_strstr(&line[i], "<<"))
		{
			new_line[j++] = 32;
			new_line[j++] = line[i++];
			new_line[j++] = line[i++];
			new_line[j++] = 32;
		}
		else if (ft_strstr(&line[i], ">") || ft_strstr(&line[i], "<") || ft_strstr(&line[i], "|"))
		{
			new_line[j++] = 32;
			new_line[j++] = line[i++];
			new_line[j++] = 32;
		}
		else
		{
			new_line[j++] = line[i++];
		}
	}
	new_line[j] = '\0';
	free(line);
	return (new_line);
}

int parse_input(char *line)
{
	char *new_line;
	t_syntax syntax;

	new_line = add_spaces(line);
	// free(line);
	if (!quotes_syntax_check(new_line))
		syntax = UNCLOSED_QUOTES;
	else
		syntax = other_syntax_check(new_line);
	
	if (syntax != SYNTAX_OK)
	{
		syntax_err_msg(syntax);
		free(new_line);
		return (1);
	}

	return (0);
}

char *read_input(const char *prompt)
{
	char	*line;

	while (true)
	{
		line = readline(prompt);
		add_history(line);
		if (!line)
			err_handle("exit");
		if (!ft_strlen(line) || empty_str(line))
		{
			free(line);
			continue ;
		}
		if (parse_input(line) == 1)
			continue;
	}
	return (line);
}