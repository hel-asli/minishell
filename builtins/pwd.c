/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 10:43:00 by oel-feng          #+#    #+#             */
/*   Updated: 2024/10/21 12:25:14 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	my_pwd(t_commands *cmnds, t_shell *shell, int flag)
{
	char	*cwd;
	char	buff[PATH_MAX + 1];

	cwd = NULL;
	if (cmnds->redirect && !flag && handle_redirections(cmnds->redirect) == -1)
	{
		shell->exit_status = EXIT_FAILURE;
		return true;
	}
	if (!getcwd(buff, PATH_MAX))
	{
		cwd = get_env("PWD", shell->env);
		if (!cwd)
			return (free(cwd), ft_putendl_fd("Error getting pwd", STDERR_FILENO), shell->exit_status = EXIT_FAILURE, true);
	}
	printf("%s --> %s\n", buff, cwd);
	free(cwd);
	shell->exit_status = EXIT_SUCCESS;
	return (true);
}
