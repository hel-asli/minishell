/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 17:55:49 by oel-feng          #+#    #+#             */
/*   Updated: 2024/09/02 15:53:26 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	sort_export(t_env **export)
{
	t_env		*curr;
	t_env		*tmp;
	char		*key;
	char		*value;

	curr = *export;
	while (curr)
	{
		tmp = curr->next;
		while (tmp)
		{
			if (curr->key && ft_strcmp(curr->key, tmp->key) > 0)
			{
				key = curr->key;
				value = curr->value;
				curr->key = tmp->key;
				curr->value = tmp->value;
				tmp->key = key;
				tmp->value = value;
			}
			tmp = tmp->next;
		}
		curr = curr->next;
	}
}

void	env_update(t_env **env, char *key, char *value)
{
	t_env	*curr;

	curr = *env;
	while (curr)
	{
		if (!ft_strcmp(curr->key, key))
		{
			free(curr->value);
			curr->value = value;
			return ;
		}
		curr = curr->next;
	}
}

bool	env_key_exist(t_env **env, char *key)
{
	t_env	*curr;

	curr = *env;
	while (curr)
	{
		if (!ft_strcmp(curr->key, key))
			return (true);
		curr = curr->next;
	}
	return (false);
}

bool	is_valid_export(char *str)
{
	int	i;

	i = 0;
	if (!ft_isalpha(str[i]) && str[i] != '_')
		return (false);
	i++;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_' && str[i] != '=' && str[i] != '"')
			return (false);
		i++;
	}
	return (true);
}

void    export_print(t_env *export)
{
	while (export)
	{
		if (export->value)
			printf("declare -x %s=\"%s\"\n", export->key, export->value);
		else if (export->equal == true)
			printf("declare -x %s\"\"\n", export->key);
		else
			printf("declare -x %s\n", export->key);
		export = export->next;
	}
}


void	build_export(t_env **export, char **ev)
{
	char	**sp;
	int		i;

	i = 0;
	*export = NULL;
	while (ev[i])
	{
		sp = ft_env_split(ev[i]);
		ft_lstadd_back(export, ft_lstnew(ft_strdup(sp[0]), ft_strdup(sp[1]), false));
		ft_free(sp);
		i++;
	}
}
void	export_env(t_env **env, t_env **export)
{
	t_env	*curr;

	curr = *env;
	while (curr)
	{
		ft_lstadd_back(export, ft_lstnew(ft_strdup(curr->key), ft_strdup(curr->value), false));
		curr = curr->next;
	}
}

bool    my_export(t_commands **cmnds, t_env **env, t_env **export)
{
	t_commands	*curr;
	// bool		equal;
	int			i;
	char		*key;
	char		*value;

	i = 1;
	curr = *cmnds;
	if (!curr->args[i])
		return (sort_export(export), export_print(*export), true);
	while (curr->args[i])
	{
		if (!is_valid_export(curr->args[i]))
		{
			printf("minishell: export: `%s': not a valid identifier\n", curr->args[i]);
			return (true);
		}
		key = ft_strdup(curr->args[i]);
		value = NULL;
		if (ft_strstr(curr->args[i], "="))
		{
			value = ft_strdup((char *)ft_strstr(curr->args[i], "=") + 1);
			key = ft_substr(curr->args[i], 0, ft_strlen(curr->args[i]) - ft_strlen(value) - 1);
		}
		// else
		// 	equal = true;
		if (!env_key_exist(env, key))
		{
			ft_lstadd_back(env, ft_lstnew(key, value, true));
			ft_lstadd_back(export, ft_lstnew(key, value, true));
		}
		else
		{
			env_update(env, key, value);
			env_update(export, key, value);
		}
		i++;
	}
	return (sort_export(export), true);
}
