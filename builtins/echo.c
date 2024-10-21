/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 19:04:16 by oel-feng          #+#    #+#             */
/*   Updated: 2024/10/21 02:49:02 by hel-asli         ###   ########.fr       */
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
	return (i == ft_strlen(str));
}

bool	my_echo(t_commands *cmnds, t_shell *shell, int flag)
{
	int			i;
	bool		opt;
	bool		wrote;
	t_commands	*curr;

	i = 0;
	opt = false;
	wrote = false;
	curr = cmnds;
	if (curr->redirect && !flag && handle_redirections(curr->redirect) == -1)
	{
		shell->exit_status = EXIT_FAILURE;
		return (true);
	}
	while (curr->args[++i])
	{
		if (ft_opt_check(curr->args[i], 'n') && !wrote)
			opt = true;
		else
		{
			ft_putstr_fd(curr->args[i],STDOUT_FILENO);
			wrote = true;
			if (curr->args[i + 1] != NULL)
				ft_putstr_fd(" ", STDOUT_FILENO);
		}
	}
	if (!opt)
		ft_putstr_fd("\n", STDOUT_FILENO);
	shell->exit_status = EXIT_SUCCESS;
	return (true);
}
