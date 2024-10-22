/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 03:55:58 by hel-asli          #+#    #+#             */
/*   Updated: 2024/10/22 10:51:56 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	err_handle(char *str)
{
	ft_putendl_fd(str, STDERR_FILENO);
	exit(EXIT_FAILURE);
}

void	err_exit(char *str)
{
	perror(str);
	exit(EXIT_FAILURE);
}

void ft_exit(t_exec *exec, t_shell *shell, int i)
{
	cmds_clear(&shell->commands);
	free_exec(exec);
	env_clear(&shell->env);
	exit(i);
}
