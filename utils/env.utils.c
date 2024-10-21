/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-feng <oel-feng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 20:44:19 by oel-feng          #+#    #+#             */
/*   Updated: 2024/10/21 21:21:36 by oel-feng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ft_env_size(t_env *env)
{
	int		i;
	t_env	*tmp;

	i = 0;
	tmp = env;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}

void	built_env(t_env **env, char **ev)
{
	char	**sp;
	int		i;

	i = 0;
	(void)env;
	sp = NULL;
	while (ev[i])
	{
		sp = ft_env_split(ev[i]);
		if (!ft_strcmp(sp[0], "OLDPWD"))
			ft_lstadd_back(env, ft_lstnew(ft_strdup(sp[0]), NULL, 0));
		else
			ft_lstadd_back(env, ft_lstnew(ft_strdup(sp[0]),
					ft_strdup(sp[1]), 0));
		ft_free(sp);
		i++;
	}
}

void	set_env(t_env **env)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	ft_lstadd_back(env, ft_lstnew(ft_strdup("PATH"),
			ft_strdup("/usr/gnu/bin:/usr/local/bin:/bin:/usr/bin:."), 0));
	ft_lstadd_back(env, ft_lstnew(ft_strdup("PWD"), ft_strdup(pwd), 0));
	ft_lstadd_back(env, ft_lstnew(ft_strdup("SHLVL"), ft_strdup("1"), 0));
	ft_lstadd_back(env, ft_lstnew(ft_strdup("_"),
			ft_strdup("/usr/bin/env"), 0));
	ft_lstadd_back(env, ft_lstnew(ft_strdup("OLDPWD"), NULL, 0));
	free(pwd);
}

char	**list_arr(t_env *env)
{
	char	**ptr;
	t_env	*tmp;
	int		i;

	i = 0;
	ptr = malloc(sizeof(char *) * (ft_env_size(env) + 1));
	if (!ptr)
		err_handle("Malloc Failure");
	tmp = env;
	while (tmp)
	{
		if (tmp->value != NULL)
			ptr[i++] = ft_strjoin_char(tmp->key, tmp->value, '=');
		tmp = tmp->next;
	}
	ptr[i] = NULL;
	return (ptr);
}

void	env_clear(t_env **env)
{
	t_env	*cur;
	t_env	*tmp;

	if (!env)
		return ;
	cur = *env;
	while (cur)
	{
		tmp = cur;
		cur = cur->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
		tmp = NULL;
	}
	*env = NULL;
}
