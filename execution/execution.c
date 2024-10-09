/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 09:53:15 by oel-feng          #+#    #+#             */
/*   Updated: 2024/10/09 22:33:20 by hel-asli         ###   ########.fr       */
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

void	free_exec(t_exec *exec)
{
	if (exec->ids)
		free(exec->ids);
	if (exec->fds)
		free(exec->fds);
}
static int handle_redirections(t_redirect *redirect)
{
    while (redirect)
    {
        int fd;
		
		fd = -1;
        if (redirect->type == INPUT || redirect->type == HEREDOC_INPUT)
        {
			if (redirect->type == INPUT)
			{
				fd = open(redirect->file, O_RDONLY);
				if (fd == -1)
				{
					ft_fprintf(2, "Error: Cannot open file for input: %s\n", redirect->file);
					return -1;
				}
			}
			else
				fd = redirect->heredoc_fd;
            if (dup2(fd, STDIN_FILENO) == -1)
            {
                ft_fprintf(2, "Error: Failed to redirect input\n");
                close(fd);
                return -1;
            }
        }
        else if (redirect->type == OUT_TRUNC || redirect->type == OUT_APPEND)
        {
            int flags = O_WRONLY | O_CREAT;
            if (redirect->type == OUT_APPEND)
            {
                flags |= O_APPEND;
            }
            else
            {
                flags |= O_TRUNC;
            }
            
            fd = open(redirect->file, flags, 0644);
            if (fd == -1)
            {
                ft_fprintf(2, "Error: Cannot open file for output: %s\n", redirect->file);
                return -1;
            }
            if (dup2(fd, STDOUT_FILENO) == -1)
            {
                ft_fprintf(2, "Error: Failed to redirect output\n");
                close(fd);
                return -1;
            }
        }
		if (fd != -1)
        	close(fd);
        redirect = redirect->next;
    }
    return 0;
}

void execute_command(t_env *env, t_commands *cmnds, t_exec *exec, int i)
{
    if (i > 0 && dup2(exec->fds[i - 1][0], STDIN_FILENO) == -1)
        err_exit("Dup2 Failure");
    if (i < exec->nbr && dup2(exec->fds[i][1], STDOUT_FILENO) == -1)
        err_exit("Dup2 Failure");
    exec_close(exec->fds, exec->nbr);
    if (handle_redirections(cmnds->redirect) == -1)
        exit(EXIT_FAILURE);

    if (builtins_check(cmnds, &env))
    {
        free_exec(exec);
        exit(EXIT_SUCCESS);
    }

    char *cmd_path = find_command(cmnds->args[0], env);
    if (cmd_path != NULL)
    {
        execve(cmd_path, cmnds->args, exec->ev_execve);
        ft_fprintf(2, "Error: Failed to execute command: %s\n", cmnds->args[0]);
        free(cmd_path);
        free_exec(exec);
        exit(EXIT_FAILURE);
    }
    
    ft_fprintf(2, "Error: Command not found: %s\n", cmnds->args[0]);
    free_exec(exec);
    exit(EXIT_FAILURE);
}

void execution_start(t_shell *shell)
{
    t_exec	exec;
    int		status;
    t_commands *cmnds;
	int		i;

    exec.ev_execve = list_arr(shell->env);
    exec.nbr = ft_lstsize(shell->commands) - 1;
    exec.ids = malloc(sizeof(pid_t) * (exec.nbr + 1));
    if (!exec.ids)
        err_handle("Malloc failure.");
    exec.fds = fds_allocation(exec.nbr);
    if (!exec.fds)
        err_handle("Malloc failure.");
    
    exec_pipe(&exec);
	i = 0;
	cmnds = shell->commands;
    while (i <= exec.nbr)
    {
        exec.ids[i] = fork();
        if (exec.ids[i] < 0)
        {
            exec_close(exec.fds, exec.nbr);
            err_exit("Fork failure");
        }
        if (exec.ids[i] == 0)
        {
            execute_command(shell->env, cmnds, &exec, i);
        }
		i++;
        cmnds = cmnds->next;
    }

    exec_close(exec.fds, exec.nbr);

	status = 0;
	i = 0;
    while (i <= exec.nbr)
    {
        if (waitpid(exec.ids[i], &status, 0) == -1 || errno == ECHILD)
            err_exit("WaitPid");
		i++;
    }

    shell->exit_status = WEXITSTATUS(status);
    free(exec.ids);
    // Free exec.ev_execve if necessary
}