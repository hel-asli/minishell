/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-feng <oel-feng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 02:45:49 by oel-feng          #+#    #+#             */
/*   Updated: 2024/10/21 22:36:54 by oel-feng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	quotes_syntax_check(char *line)
{
	bool	in_quotes;
	char	quote_type;
	int		i;

	i = 0;
	quote_type = 0;
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

t_syntax	other_syntax_check(char *line)
{
	int			i;
	t_syntax	result;
	char		**tokens;

	i = -1;
	tokens = ft_split(line);
	if (!tokens)
		err_handle("Allocation Fail");
	result = SYNTAX_OK;
	while (tokens[++i] && result == SYNTAX_OK)
	{
		if (is_redirection(tokens[i]) && !check_redirection(tokens, i))
			result = INVALID_REDIRECTINO;
		else if (!ft_strcmp(tokens[i], "<<") && !check_heredoc(tokens, i))
			result = INVALID_HEREDOC;
		else if (!ft_strcmp(tokens[i], "|") && !check_pipe(tokens, i))
			result = INVALIDE_PIPE;
	}
	return (ft_free(tokens), result);
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

char	*check_value(char *value)
{
	if (!value[0])
		return (free(value), NULL);
	return (value);
}

char	*expand_arg(char *arg, t_shell *shell)
{
	char	*value;
	bool	quotes[2];
	int		i;

	(1) && (i = 0, quotes[0] = false, quotes[1] = false);
	value = ft_strdup("");
	if (!value)
		err_handle("allocation");
	while (arg[i])
	{
		if (arg[i] == '\'' && !quotes[1])
			(1) && (quotes[0] = !quotes[0], i++);
		else if (arg[i] == '"' && !quotes[0])
			(1) && (quotes[1] = !quotes[1], i++);
		else if (arg[i] == '$' && !quotes[0])
		{
			i++;
			if ((arg[i] == '"' || arg[i] == '\'') && !quotes[1] && !quotes[0])
				continue ;
			value = ft_strjoin(value, get_new_value(shell, arg, &i));
		}
		else
			(1) && (value = str_add_char(value, arg[i]), i++);
	}
	return (check_value(value));
}
