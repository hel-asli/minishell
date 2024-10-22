/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 10:43:00 by oel-feng          #+#    #+#             */
/*   Updated: 2024/10/22 11:14:54 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	my_pwd(t_commands *cmnds, t_shell *shell, int flag)
{
	char	*cwd;
	char	buff[PATH_MAX + 1];

	cwd = NULL;
	if (cmnds->redirect && !flag && handle_redirections(cmnds->redirect) == -1)
		return (shell->exit_status = EXIT_FAILURE, true);
	if (!getcwd(buff, PATH_MAX))
	{
		cwd = get_env("PWD", shell->env);
		if (!cwd)
		{
			ft_putendl_fd("Error getting pwd", STDERR_FILENO);
			shell->exit_status = EXIT_FAILURE;
			return (free(cwd), true);
		}
	}
	if (cwd)
		ft_fprintf(STDOUT_FILENO, "%s\n", cwd);
	else
		ft_fprintf(STDOUT_FILENO, "%s\n", buff);
	(1) && (free(cwd), shell->exit_status = EXIT_SUCCESS);
	return (true);
}
