/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-feng <oel-feng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 17:55:49 by oel-feng          #+#    #+#             */
/*   Updated: 2024/10/10 17:45:31 by oel-feng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void    export_print(t_env **exp)
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

static void	sort_export(t_env **export)
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
	export_print(export);
}

static bool	env_key_exist(t_env **env, char *key)
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

static bool is_valid_export(char *str)
{
	bool equal;
	int	i;

	i = 0;
	equal = false;
	if (!ft_isalpha(str[i]) && str[i] != '_')
		return (false);
	i++;
	while (str[i])
	{
		if (str[i] == '+' && str[i + 1] != '=' && equal != true)
			return (false);
		if (!ft_isalnum(str[i]) && str[i] != '_' && str[i] != '=')
			return (false);
		if (str[i] == '+' && str[i + 1] == '=' && equal == false)
			equal = true;
		i++;
	}
	return (true);	
}


static void	env_concat(t_env **env, char *key, char *value)
{
	t_env	*curr;

	curr = *env;
	while (curr)
	{
		if (!ft_strcmp(curr->key, key))
		{
			curr->value = ft_strjoin(curr->value, value);
			curr->exported = 1337;
			return ;
		}
		curr = curr->next;
	}
}

static void	env_export(t_env **env, char *key, char *value)
{
	t_env *curr;

	curr= *env;
	while (curr)
	{
		if (!ft_strcmp(curr->key, key))
		{
			free(curr->value);
			curr->value = ft_strdup(value);
			curr->exported = 1337;
			return ;
		}
	}
}

static t_env	*export_lstnew(char *key, char *value)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = key;
	node->value = value;
	node->exported = 1337;
	node->next = NULL;
	return (node);
}

static void export_handler(t_env **export, char *args)
{
	char	*key;
	
	key = ft_strdup(args);
	if (ft_strstr(args, "+") && env_key_exist(export, key))
		env_concat(export, key, ft_strdup((char *)ft_strstr(args, "+") + 2));
	else if (ft_strstr(args, "+") && !env_key_exist(export, key))
		ft_lstadd_back(export, export_lstnew(key, ft_strdup((char *)ft_strstr(args, "=") + 2)));
	else if (ft_strstr(args, "=") && env_key_exist(export, key))
		env_export(export, key, ft_strdup((char *)ft_strstr(args, "=") + 1));
	else if (ft_strstr(args, "=") && !env_key_exist(export, key))
		ft_lstadd_back(export, export_lstnew(key, ft_strdup((char *)ft_strstr(args, "=") + 1)));
	else
		ft_lstadd_back(export, ft_lstnew(key, NULL));
}	

// bool    my_export(t_commands **cmnds, t_env **env, t_env **export)
// {
// 	t_commands	*curr;
// 	int			i;

// 	i = 1;
// 	curr = *cmnds;
// 	if (!curr->args[i]) // If no arguments, print the exported list
// 	{
// 		sort_export(export);
// 		export_print(export);
// 		return (true);
// 	}
// 	while (curr->args[i])
// 	{
// 		if (!is_valid_export(curr->args[i]))
// 		{
// 			printf("minishell: export: `%s': not a valid identifier\n", curr->args[i]);
// 			return (true);
// 		}
// 		char *key = ft_strdup(curr->args[i]);
// 		char *value = NULL;
// 		if (ft_strstr(curr->args[i], "="))
// 		{
// 			value = ft_strdup((char *)ft_strstr(curr->args[i], "=") + 1);
// 			key = ft_substr(curr->args[i], 0, ft_strlen(curr->args[i]) - ft_strlen(value) - 1);
// 		}
// 		if (!env_key_exist(env, key))
// 		{
// 			ft_lstadd_back(env, ft_lstnew(key, value));
// 			ft_lstadd_back(export, ft_lstnew(key, value));
// 		}
// 		else
// 		{
			// env_update(env, key, value);
// 			env_update(export, key, value);
// 		}
// 		free(key);
// 		if (value)
// 			free(value); // Free value if it was allocated
// 		i++;
// 	}
// 	return (sort_export(export), true);
// }

bool	my_export(t_commands *cmnds, t_env **env)
{
	t_env		**export;
	t_commands	*curr;
	int			i;

	i = 1;
	curr = cmnds;
	export = env;
	if (!curr->args[i])
		return(sort_export(export), true);
	while (curr->args[i])
	{
		if (!is_valid_export(curr->args[i]))
		{
			ft_fprintf(2, "minishell: export: `%s': not a valid identifier\n", curr->args[i]);
			return (true);
		}
		export_handler(export, curr->args[i]);
		i++;
	}
	env = export;
	return (true);
}

// void	export_env(t_env **env, t_env **export)
// {
// 	t_env	*curr;

// 	curr = *env;
// 	while (curr)
// 	{
// 		ft_lstadd_back(export, ft_lstnew(ft_strdup(curr->key), ft_strdup(curr->value)));
// 		curr = curr->next;
// 	}
// }