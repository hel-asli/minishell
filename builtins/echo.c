/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 19:04:16 by oel-feng          #+#    #+#             */
/*   Updated: 2024/10/23 04:21:36 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static bool	ft_opt_check(char *str, char c)
{
	size_t	i;

	i = 0;
	if (str[i] == '-')
		i++;
	else
		return (false);
	while (str[i] && str[i] == c)
		i++;
	if (i > 1)
		return (i == ft_strlen(str));
	return (false);
}

bool	my_echo(t_commands *cmnds, t_shell *shell, int flag)
{
	int			i;
	bool		opt;
	bool		wrote;
	t_commands	*curr;

	(1) && (i = 0, opt = false, wrote = false, curr = cmnds);
	if (curr->redirect && !flag && handle_redirections(curr->redirect) == -1)
		return (shell->exit_status = EXIT_SUCCESS, true);
	while (curr->args[++i])
	{
		if (ft_opt_check(curr->args[i], 'n') && !wrote)
			opt = true;
		else
		{
			ft_fprintf(STDOUT_FILENO, "%s", curr->args[i]);
			wrote = true;
			if (curr->args[i + 1] != NULL)
				ft_putstr_fd(" ", STDOUT_FILENO);
		}
	}
	if (!opt)
		ft_putstr_fd("\n", STDOUT_FILENO);
	return (shell->exit_status = EXIT_SUCCESS, true);
}
