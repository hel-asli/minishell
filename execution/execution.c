/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 09:53:15 by oel-feng          #+#    #+#             */
/*   Updated: 2024/10/18 00:54:44y hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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


static int	exec_pipe(t_exec *exec)
{
	int	i;

	i = 0;
	while (i < exec->nbr)
	{
		if (pipe(exec->fds[i]) == -1)
		{
            fprintf(stderr, "fail in -> %d\n", i);
			exec_close(exec->fds, i);
            free_exec(exec);
            perror("pipe Failure");
            return (1);
		}
		i++;
	}
    return (0);
}

void fds_free(int **fd, int nb)
{
    int j = 0;

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

int handle_redirections(t_redirect *redirect)
{
    while (redirect)
    {
        int fd;
		
		fd = -1;
		if (redirect->is_ambgious)
			return (ft_fprintf(STDERR_FILENO, "minishell: Ambiguous redirection for file.\n"), -1);
        if (!redirect->file)
            return (ft_fprintf(STDERR_FILENO, "minishell: No such file or directory\n"), -1);
        if (redirect->type == INPUT || redirect->type == HEREDOC_INPUT)
        {
			if (redirect->type == INPUT)
			{
				fd = open(redirect->file, O_RDONLY);
				if (fd == -1)
                {
                    perror("open");
					return -1;
                }
			}
			else
				fd = redirect->heredoc_fd;
            if (dup2(fd, STDIN_FILENO) == -1)
            {
                perror("dup2");
                close(fd);
                return -1;
            }
            close(fd);
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
                perror("open");
                return -1;
            }
            if (dup2(fd, STDOUT_FILENO) == -1)
            {
                perror("dup2");
                close(fd);
                return -1;
            }
            close(fd);
        }
        redirect = redirect->next;
    }
    return 0;
}

void execute_command(t_commands *cmnds, t_shell *shell, t_exec *exec, int i)
{
    char 		*cmd_path;
	struct stat	statbuf;

    cmd_path = NULL;
    exec->ev_execve = list_arr(shell->env);
    if (i > 0 && dup2(exec->fds[i - 1][0], STDIN_FILENO) == -1)
        err_exit("Dup2 Failure");
    if (i < exec->nbr && dup2(exec->fds[i][1], STDOUT_FILENO) == -1)
        err_exit("Dup2 Failure");
    exec_close(exec->fds, exec->nbr);
    if (handle_redirections(cmnds->redirect) == -1)
        exit(EXIT_FAILURE);
    if (!cmnds->args)
        exit(EXIT_SUCCESS);
    if (builtins_check(shell, cmnds, &shell->env, 1))
    {
        cmds_clear(&shell->commands);
        free_exec(exec);
        env_clear(&shell->env);
        exit(EXIT_SUCCESS);
    }
	if (stat_strchr(cmnds->args[0]))
	{
    	if (stat(cmnds->args[0], &statbuf) == -1)
    	{
    	    ft_fprintf(2, "minishell: %s: Not a directory\n", cmnds->args[0]);
    	    exit(126);
    	}
    	if (S_ISDIR(statbuf.st_mode))
    	{
    	    ft_fprintf(2, "minishell: %s: is a directory\n", cmnds->args[0]);
    	    exit(126);
    	}
    	if (!(statbuf.st_mode & S_IXUSR))
    	{
    	    ft_fprintf(2, "minishell: %s: Permission denied\n", cmnds->args[0]);
    	    exit(126);
    	}
	}
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

bool is_builtin(char *str)
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

void execution_start(t_shell *shell)
{
    t_exec	exec;
    int		status;
    t_commands *cmnds;
    int out;
    int in;
	int		i;

	cmnds = shell->commands;
    exec.ev_execve = NULL;
    exec.nbr = ft_lstsize(shell->commands) - 1;
    if (exec.nbr == 0 && cmnds->args && is_builtin(cmnds->args[0]))
    {
            if (cmnds->redirect)
            {
	            out = dup(STDOUT_FILENO);
	            in	= dup(STDIN_FILENO);
            }
            if (builtins_check(shell, cmnds, &shell->env, 0))
			{
                if (cmnds->redirect)
                {
	                dup2(out, STDOUT_FILENO);
	                dup2(in, STDIN_FILENO);
	                close(out);
	                close(in);
                }
                return ;
            }
    }
    exec.ids = malloc(sizeof(pid_t) * (exec.nbr + 1));
    if (!exec.ids)
        err_handle("Malloc failure.");
    exec.fds = fds_allocation(exec.nbr);
    if (!exec.fds)
        err_handle("Malloc failure.");
    if (exec_pipe(&exec))
    {
        return ;
    }
	i = 0;
    rl_signal = 0;
    while (i <= exec.nbr)
    {
        exec.ids[i] = fork();
        if (exec.ids[i] < 0)
        {
            exec_close(exec.fds, exec.nbr);
            free_exec(&exec);
            perror("Fork failure");
            return ;
        }
        if (exec.ids[i] == 0)
        {
            signal(SIGQUIT, SIG_DFL);
            signal(SIGINT, SIG_DFL);
            execute_command(cmnds, shell, &exec, i);
        }
		i++;
        cmnds = cmnds->next;
    }
	status = 0;
	i = 0;
    exec_close(exec.fds, exec.nbr);
    while (i <= exec.nbr)
    {
        if (waitpid(exec.ids[i], &status, 0) < 0 && errno == ECHILD)
            err_exit("WaitPid");
        if (WIFEXITED(status))
            shell->exit_status = WEXITSTATUS(status);
        else if (WIFSIGNALED(status))
            break ;
        i++;
    }
    free_exec(&exec);
    if (WIFSIGNALED(status))
    {
        int sig = WTERMSIG(status);
        if (sig == SIGINT && !rl_signal)
            write(STDOUT_FILENO, "\n", 1);
        if (sig == SIGQUIT)
            write(STDOUT_FILENO, "Quit: 3\n", 8);
        shell->exit_status = 128 + sig;
    }
}
