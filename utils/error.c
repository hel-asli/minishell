/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-feng <oel-feng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 03:55:58 by hel-asli          #+#    #+#             */
/*   Updated: 2024/10/23 03:29:21 by oel-feng         ###   ########.fr       */
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

void	ft_exit(t_exec *exec, t_shell *shell, int i)
{
	cmds_clear(&shell->commands);
	free_exec(exec);
	env_clear(&shell->env);
	exit(i);
}

void	ambihious_check(t_redirect *redirect, char	*file)
{
	redirect->is_ambgious = true;
	free(redirect->file);
	redirect->file = file;
	return ;
}

char	*heredoc_expand(char *str, t_shell *shell)
{
	char	*exp;
	char	*ptr;

	ptr = NULL;
	exp = expand_arg(str, shell);
	if (!exp)
	{
		free(str);
		free(exp);
		ptr = ft_strdup("");
	}
	else
	{
		free(str);
		ptr = exp;
	}
	return (ptr);
}
