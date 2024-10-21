/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-feng <oel-feng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 04:11:26 by hel-asli          #+#    #+#             */
/*   Updated: 2024/10/21 22:40:00 by oel-feng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	heredoc_helper(char *delimter, int fd, bool expanded, t_shell *shell)
{
	char	*line;
	t_env	*env;

	(1) && (line = NULL, env = shell->env);
	while (true)
	{
		line = readline("> ");
		if (!line)
		{
			free(line);
			break ;
		}
		if (!ft_strcmp(line, delimter))
			break ;
		if (expanded)
		{
			line = expand_arg(line, shell);
			if (!line)
				line = ft_strdup("");
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
}

int	file_open(t_redirect *red)
{
	int		fd;
	int		*nbr;
	char	*name;

	nbr = malloc(sizeof(char));
	if (!nbr)
		err_handle("Malloc Failure");
	name = ft_strjoin(ft_strdup("/tmp/.heredoc-"), ft_itoa((long)nbr));
	free(nbr);
	if (!name)
		err_handle("Maloc Failure");
	fd = open(name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	red->heredoc_fd = open(name, O_RDONLY);
	if (fd == -1 || red->heredoc_fd == -1)
		return (free(name), perror("open"), -1);
	if (unlink(name) < 0)
		return (free(name), perror("unlink"), -1);
	free(name);
	return (fd);
}

int	heredoc_fork(t_shell *shell, t_redirect *red, int fd)
{
	pid_t	id;
	int		status;

	id = fork();
	if (id < 0)
		return (perror("fork"), -1);
	if (id == 0)
	{
		setup_heredoc_signals();
		heredoc_helper(red->file, fd, red->expanded, shell);
		(1) && (cmds_clear(&shell->commands), close(fd));
		env_clear(&shell->env);
		exit(0);
	}
	else
	{
		(1) && (waitpid(id, &status, 0), close(fd));
		if (WIFEXITED(status) && WEXITSTATUS(status))
		{
			(1) && (close(red->heredoc_fd), close(fd));
			shell->exit_status = WEXITSTATUS(status);
			return (1);
		}
	}
	return (0);
}

int	heredoc(t_shell *shell)
{
	t_commands	*cmd;
	t_redirect	*red;
	int			heredoc_write;

	g_rl_signal = 0;
	(1) && (cmd = shell->commands, red = NULL);
	while (cmd)
	{
		red = cmd->redirect;
		while (red)
		{
			if (red->type == HEREDOC_INPUT)
			{
				heredoc_write = file_open(red);
				if (heredoc_write == -1)
					return (1);
				if (heredoc_fork(shell, red, heredoc_write))
					return (1);
			}
			red = red->next;
		}
		cmd = cmd->next;
	}
	return (0);
}

void	process_pipe_cmds(t_shell **shell, char **pipes)
{
	int			i;
	char		**tab;
	char		**args;
	t_redirect	*redirect;

	(1) && (i = 0, redirect = NULL);
	while (pipes[i])
	{
		tab = ft_split(pipes[i]);
		if (!tab)
			err_handle("Allocation Faile");
		(1) && (protect_tab(tab), redirect = build_redirection(tab));
		args = args_allocation(tab, count_non_redirection_arg_size(tab));
		(1) && (fr_args(tab), args = expand_args(args, *shell));
		expand_redirect(redirect, *shell);
		if (!args && !redirect)
		{
			i++;
			continue ;
		}
		ft_back_addlst(&(*shell)->commands, ft_newlist(args, redirect));
		i++;
	}
	fr_args(pipes);
}
