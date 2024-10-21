/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-feng <oel-feng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 16:38:37 by oel-feng          #+#    #+#             */
/*   Updated: 2024/10/21 22:09:01 by oel-feng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	my_env(t_commands *cmnds, t_shell *shell, int flag)
{
	t_env	*curr;

	curr = shell->env;
	if (cmnds->redirect && !flag && handle_redirections(cmnds->redirect) == -1)
	{
		shell->exit_status = EXIT_FAILURE;
		return (true);
	}
	if (cmnds->args[1])
	{
		ft_fprintf(2, "Too may arguments.\n");
		return (shell->exit_status = EXIT_FAILURE, true);
	}
	while (curr)
	{
		if (curr->value)
			ft_fprintf(STDOUT_FILENO, "%s=%s\n", curr->key, curr->value);
		else if (curr->exported == 0)
			ft_fprintf(STDOUT_FILENO, "%s\n", curr->key);
		curr = curr->next;
	}
	shell->exit_status = EXIT_SUCCESS;
	return (true);
}
