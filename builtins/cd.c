/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-feng <oel-feng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 23:14:16 by oel-feng          #+#    #+#             */
/*   Updated: 2024/10/10 00:04:04 by oel-feng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	env_update(t_env **env, char *key, char *value)
{
	t_env	*curr;

	curr = *env;
	while (curr)
	{
		if (!ft_strcmp(curr->key, key))
		{
			free(curr->value);
			curr->value = ft_strdup(value);
			return ;
		}
		curr = curr->next;
	}
}

bool	my_cd(t_commands *cmnds, t_env **env)
{
	t_env		*tmp;
	t_commands	*curr;
	char		*oldpwd;
	char		*pwd;

	tmp = *env;
	curr = cmnds;
	oldpwd = get_env("PWD", tmp);
	if (curr->args[1] == NULL)
	{
		char *home = get_env("HOME", tmp);
		if (home == NULL || chdir(home) == -1)
		{
			ft_putstr_fd("minishell: cd: HOME not set or invalid\n", 2);
			free(oldpwd);
			return (false);
		}
	}
	else
	{
		if (chdir(curr->args[1]) == -1)
		{
			perror("minishell: cd");
			free(oldpwd);
			return (false);
		}
	}
	pwd = getcwd(NULL, 0);
	if (pwd == NULL)
	{
		perror("minishell: cd");
		free(oldpwd);
		return (false);
	}
	env_update(env, "OLDPWD", oldpwd);
	free(oldpwd);
	oldpwd = NULL;
	env_update(env, "PWD", pwd);
	free(pwd);
	pwd = NULL;
	return (true);
}
