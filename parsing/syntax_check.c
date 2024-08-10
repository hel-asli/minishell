/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-feng <oel-feng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 02:45:49 by oel-feng          #+#    #+#             */
/*   Updated: 2024/08/08 04:55:18 by oel-feng         ###   ########.fr       */
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
