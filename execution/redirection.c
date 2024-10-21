/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-feng <oel-feng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 22:15:08 by oel-feng          #+#    #+#             */
/*   Updated: 2024/10/21 22:19:45 by oel-feng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	exec_close(int **fds, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		if (close(fds[i][0]) == -1 || close(fds[i][1]) == -1)
			err_handle("Close Failure");
		i++;
	}
}

int	exec_pipe(t_exec *exec)
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

int	input_redirection(t_redirect *redirect)
{
	int	fd;

	fd = -1;
	if (redirect->type == INPUT)
	{
		fd = open(redirect->file, O_RDONLY);
		if (fd == -1)
		{
			perror("open");
			return (-1);
		}
	}
	else
		fd = redirect->heredoc_fd;
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		return (-1);
	}
	if (fd != -1)
		close(fd);
	return (0);
}

int	out_redirection(t_redirect *redirect)
{
	int	fd;
	int	flags;

	fd = -1;
	flags = O_WRONLY | O_CREAT;
	if (redirect->type == OUT_APPEND)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(redirect->file, flags, 0644);
	if (fd == -1)
		return (perror("open"), -1);
	if (dup2(fd, STDOUT_FILENO) == -1)
		return (perror("dup2"), close(fd), -1);
	if (fd != -1)
		close(fd);
	return (0);
}

int	handle_redirections(t_redirect *redirect)
{
	int	fd;

	while (redirect)
	{
		fd = -1;
		if (redirect->is_ambgious)
			return (ft_fprintf(STDERR_FILENO,
					"minishell: Ambiguous redirection for file.\n"), -1);
		if (!redirect->file)
			return (ft_fprintf(STDERR_FILENO,
					"minishell: No such file or directory\n"), -1);
		if (redirect->type == INPUT || redirect->type == HEREDOC_INPUT)
		{
			if (input_redirection(redirect) == -1)
				return (-1);
		}
		else if (redirect->type == OUT_TRUNC || redirect->type == OUT_APPEND)
		{
			if (out_redirection(redirect) == -1)
				return (-1);
		}
		redirect = redirect->next;
	}
	return (0);
}
