/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-feng <oel-feng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 17:55:49 by oel-feng          #+#    #+#             */
/*   Updated: 2024/10/10 00:35:17 by oel-feng         ###   ########.fr       */
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

void    export_print(t_env **exp)
{
	t_env	*export;

	export = *exp;
	while (export)
	{
		if (export->value)
			printf("declare -x %s=\"%s\"\n", export->key, export->value);
		else if (export->equal == true)
			printf("declare -x %s=\"\"\n", export->key);
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
		if (sp[1] == NULL) // Check if value is NULL
			ft_lstadd_back(export, ft_lstnew(ft_strdup(sp[0]), NULL));
		else
			ft_lstadd_back(export, ft_lstnew(ft_strdup(sp[0]), ft_strdup(sp[1])));
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
		ft_lstadd_back(export, ft_lstnew(ft_strdup(curr->key), ft_strdup(curr->value)));
		curr = curr->next;
	}
}

bool    my_export(t_commands **cmnds, t_env **env, t_env **export)
{
	t_commands	*curr;
	int			i;

	i = 1;
	curr = *cmnds;
	if (!curr->args[i]) // If no arguments, print the exported list
	{
		sort_export(export);
		export_print(export);
		return (true);
	}
	while (curr->args[i])
	{
		if (!is_valid_export(curr->args[i]))
		{
			printf("minishell: export: `%s': not a valid identifier\n", curr->args[i]);
			return (true);
		}
		char *key = ft_strdup(curr->args[i]);
		char *value = NULL;
		if (ft_strstr(curr->args[i], "="))
		{
			value = ft_strdup((char *)ft_strstr(curr->args[i], "=") + 1);
			key = ft_substr(curr->args[i], 0, ft_strlen(curr->args[i]) - ft_strlen(value) - 1);
		}
		if (!env_key_exist(env, key))
		{
			ft_lstadd_back(env, ft_lstnew(key, value));
			ft_lstadd_back(export, ft_lstnew(key, value));
		}
		else
		{
			env_update(env, key, value);
			env_update(export, key, value);
		}
		free(key);
		if (value)
			free(value); // Free value if it was allocated
		i++;
	}
	return (sort_export(export), true);
}
