/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-feng <oel-feng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 23:14:16 by oel-feng          #+#    #+#             */
/*   Updated: 2024/10/01 17:29:05 by oel-feng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	my_cd(t_commands **cmnds, t_env **env)
{
	t_env		*tmp;
	t_commands	*curr;
	char		*oldpwd;
	char		*pwd;

	tmp = *env;
	curr = *cmnds;
	oldpwd = get_env("PWD", tmp);
	if (curr->args[1] == NULL)
	{
		char *home = get_env("HOME", tmp);
		if (home == NULL || chdir(home) == -1)
		{
			printf("minishell: cd: HOME not set or invalid\n");
			return (false);
		}
	}
	else
	{
		if (chdir(curr->args[1]) == -1)
		{
			perror("minishell: cd");
			return (false);
		}
	}
	pwd = getcwd(NULL, 0);
	if (pwd == NULL)
	{
		perror("minishell: cd");
		return (false);
	}
	env_update(env, "OLDPWD", oldpwd);
	free(oldpwd);
	env_update(env, "PWD", pwd);
	free(pwd);
	return (true);
}
