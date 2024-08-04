/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 03:48:06 by hel-asli          #+#    #+#             */
/*   Updated: 2024/08/05 00:39:29 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Include/parsing.h"

bool quotes_syntax_check(char *line)
{
	bool in_quotes = false;
	char	quote_type = 0;
	int i = 0;

	while (line[i])
	{
		if (!in_quotes && (line[i] == 39 || line[i] == 34))
		{
			in_quotes = true;
			quote_type = line[i];
		}
		else if (in_quotes && line[i] == quote_type)
			in_quotes = false;
		i++;
	}
	return (!in_quotes);
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
bool is_redirection(char *token)
{
	return (!ft_strcmp(token, ">>") || !ft_strcmp(token, ">") || !ft_strcmp(token, "<"));
}

bool check_redirection(char **tokens, int i)
{
	if (tokens[i + 1] == NULL)
		return (false);
	if (tokens[i + 1] && (is_redirection(tokens[i + 1]) || !ft_strcmp(tokens[i + 1], "<<") || !ft_strcmp(tokens[i + 1], "|")))
		return (false);
	return (true);
}

bool check_heredoc(char **tokens, int i)
{
	if (tokens[i + 1] == NULL)
		return (false);
	if (tokens[i + 1] && (is_redirection(tokens[i + 1]) || !ft_strcmp(tokens[i + 1], "<<") || !ft_strcmp(tokens[i + 1], "|")))
		return (false);
	return (true);
}

bool check_pipe(char **tokens, int i)
{
	if (i == 0 || tokens[i + 1] == NULL)
		return (false);
	else if (i > 0 && !ft_strcmp(tokens[i - 1], "|"))
		return (false);
	return (true);
}


int check_long(char **tokens, int i)
{
	if (tokens[i + 1] && !ft_strcmp(tokens[i], ">>") && (!ft_strcmp(tokens[i + 1], ">>") || !ft_strcmp(tokens[i + 1], ">")))
		return (1);
	else if (tokens[i + 1] && !ft_strcmp(tokens[i], "<<") && (!ft_strcmp(tokens[i + 1], "<<") || !ft_strcmp(tokens[i + 1], "<")))
		return (2);
	return (0);
}

t_syntax other_syntax_check(char *line)
{
	t_syntax result;
	int i = -1;
	char **tokens = ft_split(line);
	if (!tokens)
		err_handle("Allocation Fail");
	
	result = SYNTAX_OK;
	while (tokens[++i])
	{
		if (is_redirection(tokens[i]) && !check_redirection(tokens, i))
		{
			result = INVALID_REDIRECTINO;
			break ;
		}
		else if (!ft_strcmp(tokens[i], "<<") && !check_heredoc(tokens, i))
		{
			result = INVALID_HEREDOC;
			break ;
		}
		else if (!ft_strcmp(tokens[i], "|") && !check_pipe(tokens, i))
		{
			result = INVALIDE_PIPE;
			break ;
		}
	}
	return (ft_free(tokens), result);
}

void syntax_err_msg(t_syntax syntax)
{
	if (syntax == INVALID_REDIRECTINO)
		ft_putendl_fd(SYNTAX_REDIRECTION, STDERR_FILENO);
	else if (syntax == UNCLOSED_QUOTES)
		ft_putendl_fd(SYNTAX_QUOTES, STDERR_FILENO);
	else if (syntax == INVALIDE_PIPE)
		ft_putendl_fd(SYNTAX_PIPE, STDERR_FILENO);
	else if (syntax == INVALID_APPEND)
		ft_putendl_fd(SYNTAX_INVALID_APPEND, STDERR_FILENO);
	else if (syntax == INVALID_HEREDOC)
		ft_putendl_fd(SYNTAX_HEREDOC, STDERR_FILENO);
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
	size_t j = 0;
	size_t i = 0;

	char *new_line = malloc(sizeof(char) * len + 1);
	if (!new_line)
		return (free(line), NULL);
	while (line[i] && j < len)
	{
		if (ft_strstr(&line[i], ">>") || ft_strstr(&line[i], "<<"))
		{
			(new_line[j++] = 32, new_line[j++] = line[i++]);
			(new_line[j++] = line[i++],new_line[j++] = 32);
		}
		else if (ft_strstr(&line[i], ">") || ft_strstr(&line[i], "<") || ft_strstr(&line[i], "|"))
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

int parse_input(char *line)
{
	char *new_line;
	t_syntax syntax;

	new_line = add_spaces(line);
	if (!new_line)
		return (-1);
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