/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-feng <oel-feng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 09:53:15 by oel-feng          #+#    #+#             */
/*   Updated: 2024/08/25 04:31:17 by oel-feng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	builtins_check(t_commands **cmnds, t_env **env, t_env **export)
{
	t_commands	*curr;

	curr = *cmnds;
	// remove_extra_quotes(curr);
	if (!ft_strcmp(curr->cmd, "cd"))
		return (my_cd(cmnds, env));
	else if (!ft_strcmp(curr->cmd, "echo"))
		return (my_echo(cmnds));
	else if (!ft_strcmp(curr->cmd, "env"))
		return (my_env(env));
	else if (!ft_strcmp(curr->cmd, "exit"))
		return (my_exit(cmnds));
	else if (!ft_strcmp(curr->cmd, "export"))
		return (my_export(cmnds, env, export));
	else if (!ft_strcmp(curr->cmd, "pwd"))
		return (my_pwd());
	else if (!ft_strcmp(curr->cmd, "unset"))
		return (my_unset(cmnds, env, export));
	else
		return (false);
}

void execution_start(t_shell *shell, char **ev)
{
    int tmp;
    
    if (builtins_check(&shell->commands, &shell->env, &shell->export))
        return;
    else {
        tmp = dup(0);
        if (tmp == -1)
        {
            ft_putstr_fd("Error: Failed to duplicate file descriptor\n", 2);
            return;
        }
        while (shell->commands)
        {
            if (execute(&shell->commands, ev, &tmp) != 0)
            {
                ft_putstr_fd("Error executing\n", 2);
                break;
            }
        }
        close(tmp);
    }
}
