/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 00:21:37 by hel-asli          #+#    #+#             */
/*   Updated: 2024/08/08 00:23:15 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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