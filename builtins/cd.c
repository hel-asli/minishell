/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-feng <oel-feng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 23:14:16 by oel-feng          #+#    #+#             */
/*   Updated: 2024/10/22 03:22:30 by oel-feng         ###   ########.fr       */
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

bool	is_exists(char *key, t_env *env)
{
	while (env)
	{
		if (!ft_strcmp(env->key, key))
			return (true);
		env = env->next;
	}
	return (false);
}

bool	my_cd(t_commands *cmnds, t_shell *shell, t_env **env, int flag)
{
	t_env		*tmp;
	t_commands	*curr;
	char		*oldpwd;
	char		*pwd;
	char		*home;

	tmp = *env;
	curr = cmnds;
	pwd = NULL;
	oldpwd = get_env("PWD", tmp);
	if (curr->redirect && !flag && handle_redirections(curr->redirect) == -1)
	{
		shell->exit_status = EXIT_FAILURE;
		free(oldpwd);
		return (true);
	}
	if (curr->args[1] == NULL)
	{
		home = get_env("HOME", tmp);
		if (!home)
			return (ft_putstr_fd("minishell: HOME not set\n", STDERR_FILENO),
				free(oldpwd), shell->exit_status = EXIT_FAILURE, true);
		else
		{
			if (chdir(home) < 0)
			{
				shell->exit_status = EXIT_FAILURE;
				return (perror("HOME"), free(oldpwd), free(home), true);
			}
			if (oldpwd)
			{
				if (!is_exists("OLDPWD", tmp))
					ft_lstadd_back(env, ft_lstnew(ft_strdup("OLDPWD"),
							ft_strdup(oldpwd), 0));
				else
					env_update(env, "OLDPWD", oldpwd);
			}
			pwd = getcwd(NULL, 0);
			if (!pwd)
				return (perror("getcwd"), free(home), free(oldpwd), true);
			if (is_exists("PWD", tmp))
				env_update(env, "PWD", pwd);
			else
				ft_lstadd_back(env, ft_lstnew(ft_strdup("PWD"),
						ft_strdup(pwd), 0));
			return (free(pwd), free(oldpwd), free(home), true);
		}
	}
	else
	{
		if (chdir(curr->args[1]) == -1)
		{
			shell->exit_status = EXIT_FAILURE;
			return (perror("minishell: cd"), free(oldpwd), true);
		}
		if (oldpwd)
		{
			if (!is_exists("OLDPWD", tmp))
				ft_lstadd_back(env, ft_lstnew(ft_strdup("OLDPWD"),
						ft_strdup(oldpwd), 0));
			else
				env_update(env, "OLDPWD", oldpwd);
		}
		pwd = getcwd(NULL, 0);
		if (!pwd)
		{
			if (ft_strcmp(curr->args[1], "..") || ft_strcmp(curr->args[1], "."))
				pwd = ft_strjoin_char(oldpwd, curr->args[1], '/');
			else
				return (perror("getcwd"), free(oldpwd), true);
		}
		if (is_exists("PWD", tmp))
			env_update(env, "PWD", pwd);
		else
			ft_lstadd_back(env, ft_lstnew(ft_strdup("PWD"), ft_strdup(pwd), 0));
		shell->exit_status = 0;
		return (free(pwd), free(oldpwd), true);
	}
}
