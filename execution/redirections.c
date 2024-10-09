/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 21:36:02 by oel-feng          #+#    #+#             */
/*   Updated: 2024/10/09 22:23:45 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// int handle_redirections(t_redirect *redirects)
// {
// 	t_redirect *curr = redirects;
// 	int fd;

// 	while (curr)
// 	{
// 		if (curr->is_ambgious)
// 			return (ft_fprintf(2, "Error: Ambiguous redirection for file.\n"), -1);
// 		if (curr->type == OUT_TRUNC)
// 		{
// 			fd = open(curr->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 			if (fd == -1 || dup2(fd, STDOUT_FILENO) == -1)
// 				return (ft_fprintf(2, "Error\n"), -1);
// 			close(fd);
// 		}
// 		else if (curr->type == OUT_APPEND)
// 		{
// 			fd = open(curr->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
// 			if (fd == -1 || dup2(fd, STDOUT_FILENO) == -1)
// 				return (ft_fprintf(2, "Error\n"), -1);
// 			close(fd);
// 		}
// 		else if (curr->type == INPUT)
// 		{
// 			fd = open(curr->file, O_RDONLY);;
// 			if (fd == -1 || dup2(fd, STDIN_FILENO) == -1)
// 				return (ft_fprintf(2, "Error\n"), -1);
// 			close(fd);
// 		}
// 		curr = curr->next;
// 	}
// 	return 0;
// }
