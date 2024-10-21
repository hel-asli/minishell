/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_helper.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-feng <oel-feng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 22:14:33 by oel-feng          #+#    #+#             */
/*   Updated: 2024/10/21 22:23:06 by oel-feng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	is_builtin(char *str)
{
	if (str)
	{
		if (!ft_strcmp(str, "cd"))
			return (true);
		else if (!ft_strcmp(str, "echo"))
			return (true);
		else if (!ft_strcmp(str, "env"))
			return (true);
		else if (!ft_strcmp(str, "pwd"))
			return (true);
		else if (!ft_strcmp(str, "exit"))
			return (true);
		else if (!ft_strcmp(str, "unset"))
			return (true);
		else if (!ft_strcmp(str, "export"))
			return (true);
		else
			return (false);
	}
	return (false);
}

bool	builtins_check(t_shell *shell, t_commands *cmnds, t_env **env, int flag)
{
	t_commands	*curr;

	curr = cmnds;
	if (curr->args[0])
	{
		if (!ft_strcmp(curr->args[0], "cd"))
			return (my_cd(cmnds, shell, env, flag));
		else if (!ft_strcmp(curr->args[0], "echo"))
			return (my_echo(cmnds, shell, flag));
		else if (!ft_strcmp(curr->args[0], "env"))
			return (my_env(cmnds, shell, flag));
		else if (!ft_strcmp(curr->args[0], "pwd"))
			return (my_pwd(cmnds, shell, flag));
		else if (!ft_strcmp(curr->args[0], "exit"))
			return (my_exit(cmnds, shell, flag));
		else if (!ft_strcmp(curr->args[0], "unset"))
			return (my_unset(cmnds, shell, env, flag));
		else if (!ft_strcmp(curr->args[0], "export"))
			return (my_export(cmnds, shell, &shell->env, flag));
		else
			return (false);
	}
	return (false);
}

void	fds_free(int **fd, int nb)
{
	int	j;

	j = 0;
	if (fd)
	{
		while (j < nb)
			free(fd[j++]);
		free(fd);
	}
}

int	**fds_allocation(int nb)
{
	int		j;
	int		**fds;

	fds = malloc(sizeof(int *) * (nb));
	if (!fds)
		return (NULL);
	j = 0;
	while (j < nb)
	{
		fds[j] = malloc(sizeof(int) * 2);
		if (!fds[j])
			err_handle("Malloc Failure");
		j++;
	}
	return (fds);
}

void	free_exec(t_exec *exec)
{
	if (exec->ids)
		free(exec->ids);
	if (exec->fds)
		fds_free(exec->fds, exec->nbr);
	if (exec->ev_execve)
		fr_args(exec->ev_execve);
}
