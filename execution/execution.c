/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-feng <oel-feng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 09:53:15 by oel-feng          #+#    #+#             */
/*   Updated: 2024/10/09 21:16:36 by oel-feng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	builtins_check(t_commands *cmnds, t_env **env)
{
	t_commands	*curr;

	curr = cmnds;
	if (!ft_strcmp(curr->cmd, "cd"))
		return (my_cd(cmnds, env));
	else if (!ft_strcmp(curr->cmd, "echo"))
		return (my_echo(cmnds));
	else if (!ft_strcmp(curr->cmd, "env"))
		return (my_env(env));
	else if (!ft_strcmp(curr->cmd, "exit"))
		return (my_exit(cmnds));
	else if (!ft_strcmp(curr->cmd, "pwd"))
		return (my_pwd());
	else
		return (false);
	// else if (!ft_strcmp(curr->cmd, "export"))
	// 	return (my_export(cmnds, env));
	// else if (!ft_strcmp(curr->cmd, "unset"))
	// 	return (my_unset(cmnds, env));
}

static void	exec_close(int **fds, int size)
{
	int i;

	i = 0;
	while (i < size)
	{
		if (close(fds[i][0]) == -1 || close(fds[i][1]) == -1)
			err_handle("Close Failure");
		i++;
	}
}

static void	exec_pipe(t_exec *exec)
{
	int	i;

	i = 0;
	while (i < exec->nbr)
	{
		if (pipe(exec->fds[i]) == -1)
		{
			// free(all);
			exec_close(exec->fds, i);
			err_exit("Pipe Failure");
		}
		i++;
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

// static void	exec_init(t_shell *shell)
// {
// 	t_exec exec = shell->exec;
	
// 	exec.ev_execve = list_arr(shell->env);
// 	exec.nbr = ft_lstsize(shell->commands);
// 	exec.ids = malloc(sizeof(pid_t) * (exec.nbr + 1));
// 	if (!exec.ids)
// 		err_handle("Malloc failure.");
// 	exec.fds = fds_allocation(exec.nbr);
// 	if (!exec.fds)
// 		err_handle("Malloc failure.");
// }

void	free_exec(t_exec *exec)
{
	if (exec->ids)
		free(exec->ids);
	if (exec->fds)
		free(exec->fds);
}

void execute_other(t_env *env, t_commands *cmnds, t_exec *exec, int i)
{
	char *cmd_path;
	
	if (handle_redirections(cmnds->redirect) == -1)
		err_exit("Redirection Failure");
	if (dup2(exec->fds[i - 1][0], STDIN_FILENO) < 0 || dup2(exec->fds[i][1], STDOUT_FILENO) < 0)
		err_exit("Dup2 Failure");
	exec_close(exec->fds, exec->nbr);
	if (builtins_check(cmnds, &env))
	{
		free_exec(exec);
		exit(EXIT_SUCCESS);
	}
	cmd_path = find_command(cmnds->args[0], env);
	if (cmd_path != NULL)
	{
		execve(cmd_path, cmnds->args, exec->ev_execve);
		free_exec(exec);
		err_exit("Execve Other");
	}
	free_exec(exec);
	err_exit("Access Other");
}

void execute_last(t_env *env, t_commands *cmnds, t_exec *exec, int i)
{
	char *cmd_path;
	
	if (handle_redirections(cmnds->redirect) == -1)
		err_exit("Redirection Failure");
	if (dup2(exec->fds[i - 1][0], STDIN_FILENO) == -1)
		err_exit("Dup2 Failure");
	exec_close(exec->fds, exec->nbr);
	if (builtins_check(cmnds, &env))
	{
		free_exec(exec);
		exit(EXIT_SUCCESS);
	}
	cmd_path = find_command(cmnds->args[0], env);
	if (cmd_path != NULL)
	{
		execve(cmd_path, cmnds->args, exec->ev_execve);
		free_exec(exec);
		err_exit("Execve Last");
	}
	free_exec(exec);
	err_exit("Access Last");
}

void execute_first(t_env *env, t_commands *cmnds, t_exec *exec, int i)
{
	char *cmd_path;
	
	if (handle_redirections(cmnds->redirect) == -1)
		err_exit("Redirection Failure");
	if (dup2(exec->fds[i][1], STDOUT_FILENO) == -1)
		err_exit("Dup2 Failure");
	exec_close(exec->fds, exec->nbr);
	if (builtins_check(cmnds, &env))
	{
		free_exec(exec);
		exit(EXIT_SUCCESS);
	}
	cmd_path = find_command(cmnds->args[0], env);
	if (cmd_path != NULL)
	{
		execve(cmd_path, cmnds->args, exec->ev_execve);
		free_exec(exec);
		err_exit("Execve First");
	}
	free_exec(exec);
	err_exit("Access First");
}

void exec_multi(t_shell *shell, t_exec *exec)
{
	t_commands *cmnds = shell->commands;
	int	i;
	
	i = 0;
	while (i < exec->nbr)
	{
		exec->ids[i] = fork();
		if (exec->ids[i] < 0)
		{
			exec_close(exec->fds, exec->nbr);
			err_exit("Fork failure");
		}
		if (exec->ids[i] == 0)
		{
			if (i == 0)
				execute_first(shell->env, cmnds, exec, i);
			else if (i == exec->nbr)
				execute_last(shell->env, cmnds, exec, i);
			else
				execute_other(shell->env, cmnds, exec, i);
		}
		cmnds = cmnds->next;
		i++;
	}
}

void execution_start(t_shell *shell)
{
	int		i;
	int		status;
	t_exec 	exec;

	exec.ev_execve = list_arr(shell->env);
	exec.nbr = ft_lstsize(shell->commands) - 1;
	exec.ids = malloc(sizeof(pid_t) * (exec.nbr + 1));
	if (!exec.ids)
		err_handle("Malloc failure.");
	exec.fds = fds_allocation(exec.nbr);
	if (!exec.fds)
		err_handle("Malloc failure.");
	exec_pipe(&exec);
	exec_multi(shell, &exec);
	exec_close(exec.fds, exec.nbr);
	// free_all;
	i = 0;
	while (i < exec.nbr)
	{
		if (waitpid(exec.ids[i], &status, 0) == -1 || errno == ECHILD)
			err_exit("WaitPid");
		i++;
	}
	shell->exit_status = WEXITSTATUS(status);
}
