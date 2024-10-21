/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_functions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-feng <oel-feng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 22:24:18 by oel-feng          #+#    #+#             */
/*   Updated: 2024/10/21 22:28:56 by oel-feng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	is_stat(char *cmd)
{
	struct stat	statbuf;

	if (stat(cmd, &statbuf) == -1)
	{
		ft_fprintf(2, "minishell: %s: Not a directory\n", cmd);
		exit(126);
	}
	if (S_ISDIR(statbuf.st_mode))
	{
		ft_fprintf(2, "minishell: %s: is a directory\n", cmd);
		exit(126);
	}
	if (!(statbuf.st_mode & S_IXUSR))
	{
		ft_fprintf(2, "minishell: %s: Permission denied\n", cmd);
		exit(126);
	}
}

void	setup_child_signal(void)
{
	signal(SIGQUIT, SIG_DFL);
	signal(SIGINT, SIG_DFL);
}

void	signal_helper(t_shell *shell, int status)
{
	int	sig;

	sig = WTERMSIG(status);
	if (sig == SIGINT && !g_rl_signal)
	{
		write(STDOUT_FILENO, "\n", 1);
		shell->exit_status = 128 + sig;
	}
	if (sig == SIGQUIT)
	{
		write(STDOUT_FILENO, "Quit: 3\n", 8);
		shell->exit_status = 128 + sig;
	}
}

void	execute_command_helper(t_commands *cmnds, t_shell *shell,
					t_exec *exec, int i)
{
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
}

int	builtin_execute(t_shell *shell)
{
	t_commands	*cmnds;
	int			out;
	int			in;

	cmnds = shell->commands;
	if (ft_lstsize(cmnds) == 1 && cmnds->args && is_builtin(cmnds->args[0]))
	{
		if (cmnds->redirect)
		{
			out = dup(STDOUT_FILENO);
			in = dup(STDIN_FILENO);
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
			return (1);
		}
	}
	return (0);
}
