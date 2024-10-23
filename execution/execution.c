/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 09:53:15 by oel-feng          #+#    #+#             */
/*   Updated: 2024/10/23 04:20:14 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	execute_command(t_commands *cmnds, t_shell *shell, t_exec *exec, int i)
{
	char	*cmd_path;

	cmd_path = NULL;
	execute_command_helper(cmnds, shell, exec, i);
	if (!ft_strncmp(cmnds->args[0], "/", 1) || !ft_strncmp(cmnds->args[0],
			"./", 2) || !ft_strncmp(cmnds->args[0], "../", 3))
		is_stat(cmnds->args[0]);
	cmd_path = find_command(cmnds->args[0], shell->env);
	if (cmd_path)
	{
		execve(cmd_path, cmnds->args, exec->ev_execve);
		if (cmd_path)
			free(cmd_path);
		free_exec(exec);
		env_clear(&shell->env);
		cmds_clear(&shell->commands);
		err_exit("execve");
	}
	ft_fprintf(2, "Error: Command not found %s\n", cmnds->args[0]);
	free_exec(exec);
	cmds_clear(&shell->commands);
	env_clear(&shell->env);
	exit(127);
}

void	init_exec(t_exec *exec, t_shell *shell)
{
	exec->ev_execve = NULL;
	exec->nbr = ft_lstsize(shell->commands) - 1;
	exec->ids = malloc(sizeof(pid_t) * (exec->nbr + 1));
	if (!exec->ids)
		err_handle("Malloc failure.");
	exec->fds = fds_allocation(exec->nbr);
	if (!exec->fds)
		err_handle("Malloc failure.");
}

void	cmnds_wait(t_shell *shell, t_exec *exec)
{
	int	status;
	int	i;

	(1) && (i = 0, shell->exit_status = 0);
	exec_close(exec->fds, exec->nbr);
	while (i <= exec->nbr)
	{
		status = 0;
		if (waitpid(exec->ids[i], &status, 0) < 0 && errno == ECHILD)
		{
			perror("WaitPid");
			free_exec(exec);
			cmds_clear(&shell->commands);
			env_clear(&shell->env);
		}
		if (WIFEXITED(status))
			shell->exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			signal_helper(shell, status);
		i++;
	}
	free_exec(exec);
}

void	cmnds_fork(t_shell *shell, t_exec *exec)
{
	int			i;
	t_commands	*cmnds;

	i = 0;
	cmnds = shell->commands;
	while (i <= exec->nbr)
	{
		exec->ids[i] = fork();
		if (exec->ids[i] < 0)
		{
			exec_close(exec->fds, exec->nbr);
			free_exec(exec);
			perror("Fork failure");
			return ;
		}
		if (exec->ids[i] == 0)
		{
			setup_child_signal();
			execute_command(cmnds, shell, exec, i);
		}
		(1) && (i++, cmnds = cmnds->next);
	}
}

void	execution_start(t_shell *shell)
{
	t_exec	exec;

	if (builtin_execute(shell))
		return ;
	init_exec(&exec, shell);
	if (exec_pipe(&exec))
		return ;
	g_rl_signal = 0;
	cmnds_fork(shell, &exec);
	cmnds_wait(shell, &exec);
}
