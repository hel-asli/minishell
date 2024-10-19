/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 10:43:00 by oel-feng          #+#    #+#             */
/*   Updated: 2024/10/19 01:26:15 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	my_pwd(t_commands *cmnds, t_shell *shell, int flag)
{
	char	*cwd;
	char	buff[PATH_MAX + 1];

	if (cmnds->redirect && !flag && handle_redirections(cmnds->redirect) == -1)
	{
		shell->exit_status = EXIT_FAILURE;
		return true;
	}
	cwd = getcwd(buff, PATH_MAX + 1);
	if (!cwd)
		return (ft_putendl_fd("Error getting pwd", STDERR_FILENO), shell->exit_status = EXIT_FAILURE, true);
	printf("%s\n", cwd);
	shell->exit_status = EXIT_SUCCESS;
	return (true);
}
