/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-feng <oel-feng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 23:14:16 by oel-feng          #+#    #+#             */
/*   Updated: 2024/10/23 09:46:01 by oel-feng         ###   ########.fr       */
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
	t_env	*tmp;
	char	*oldpwd;

	tmp = *env;
	oldpwd = get_env("PWD", tmp);
	if (cmnds->redirect && !flag && handle_redirections(cmnds->redirect) == -1)
	{
		shell->exit_status = EXIT_FAILURE;
		free(oldpwd);
		return (true);
	}
	if (cmnds->args[1] == NULL)
		return (cd_home(env, tmp, oldpwd, &shell->exit_status));
	return (cd_home(env, tmp, oldpwd, &shell->exit_status));
}
