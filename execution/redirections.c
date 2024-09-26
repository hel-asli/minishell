/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-feng <oel-feng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 21:36:02 by oel-feng          #+#    #+#             */
/*   Updated: 2024/09/26 03:11:26 by oel-feng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int handle_redirections(t_redirect *redirects)
{
	t_redirect *curr = redirects;
	int fd;

	while (curr)
	{
		if (curr->type == OUT_TRUNC)
		{
			fd = open(curr->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd == -1)
				return (ft_fprintf(2, "Error: Cannot open file %s\n", curr->file), -1);
			if (dup2(fd, STDOUT_FILENO) == -1)
				return (ft_fprintf(2, "Error: dup2 failed\n"), -1);
			close(fd);
		}
		else if (curr->type == OUT_APPEND)
		{
			fd = open(curr->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd == -1)
				return (ft_fprintf(2, "Error: Cannot open file %s\n", curr->file), -1);
			if (dup2(fd, STDOUT_FILENO) == -1)
				return (ft_fprintf(2, "Error: dup2 failed\n"), -1);
			close(fd);
		}
		else if (curr->type == INPUT)
		{
			fd = open(curr->file, O_RDONLY);
			if (fd == -1)
				return (ft_fprintf(2, "Error: Cannot open file %s\n", curr->file), -1);
			if (dup2(fd, STDIN_FILENO) == -1)
				return (ft_fprintf(2, "Error: dup2 failed\n"), -1);
			close(fd);
		}

		curr = curr->next;
	}
	return 0;
}
