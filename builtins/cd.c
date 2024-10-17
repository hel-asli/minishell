/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 23:14:16 by oel-feng          #+#    #+#             */
/*   Updated: 2024/10/17 04:56:54 by hel-asli         ###   ########.fr       */
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
			if (!value)
				curr->value = NULL;
			else
				curr->value = ft_strdup(value);
			return ;
		}
		curr = curr->next;
	}
}
bool is_exists(char *key, t_env *env)
{
	while (env)
	{
		if (!ft_strcmp(env->key, key))
			return (true);
		env = env->next;
	}
	return (false);
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
		if (!home)
			return (ft_putstr_fd("minishell: HOME not set\n", STDERR_FILENO), true);
		else
		{
			if (chdir(home) < 0)
				err_exit("HOME");
			if (oldpwd)
			{
				if (!is_exists("OLDPWD", tmp))
					ft_lstadd_back(env, ft_lstnew(ft_strdup("OLDPWD"), ft_strdup(oldpwd)));
				else
					env_update(env, "OLDPWD", oldpwd);
			}
			pwd = getcwd(NULL, 0);
			if (is_exists("PWD", tmp))
				env_update(env, "PWD", pwd);
			else
				ft_lstadd_back(env, ft_lstnew(ft_strdup("PWD"), ft_strdup(pwd)));
			return (free(pwd), true);
		}
	}
	else
	{
		if (chdir(curr->args[1]) == -1)
		{
			perror("minishell: cd");
			free(oldpwd);
			return (true);
		}
		if (oldpwd)
		{
			if (!is_exists("OLDPWD", tmp))
				ft_lstadd_back(env, ft_lstnew(ft_strdup("OLDPWD"), ft_strdup(oldpwd)));
			else
				env_update(env, "OLDPWD", oldpwd);
		}
		pwd = getcwd(NULL, 0);
		if (is_exists("PWD", tmp))
			env_update(env, "PWD", pwd);
		else
			ft_lstadd_back(env, ft_lstnew(ft_strdup("PWD"), ft_strdup(pwd)));
		return (free(pwd), true);
	}
}
