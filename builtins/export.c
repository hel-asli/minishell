/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-feng <oel-feng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 17:55:49 by oel-feng          #+#    #+#             */
/*   Updated: 2024/10/21 22:08:24 by oel-feng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	is_valid_export(char *str)
{
	int	i;

	i = 0;
	if (!ft_isalpha(str[i]) && str[i] != '_')
		return (false);
	i++;
	while (str[i])
	{
		if (str[i] == '+' && str[i + 1] == '=')
			break ;
		if (str[i] == '=')
			break ;
		if (str[i] == '+' && str[i + 1] != '=')
			return (false);
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (false);
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
			curr->exported = 0;
			return ;
		}
		curr = curr->next;
	}
}

static void	env_export(t_env **env, char *key, char *value, int exported)
{
	t_env	*curr;

	curr = *env;
	while (curr)
	{
		if (!ft_strcmp(curr->key, key))
		{
			free(curr->value);
			curr->value = ft_strdup(value);
			curr->exported = exported;
			free(value);
			return ;
		}
		curr = curr->next;
	}
}

static void	export_handler(t_env **export, char *args)
{
	char	**key;
	char	*new_key;

	key = ft_env_split(args);
	if (num_count(key[0]) > 1)
	{
		ft_fprintf(2, "minishell: export: `%s': not a valid identifier\n",
			key[0]);
		ft_free(key);
		return ;
	}
	new_key = copy_key(key[0]);
	if (env_key_exist(export, new_key))
	{
		if (ft_lookup(args, '+'))
			env_concat(export, new_key, ft_strdup(key[1]));
		else if (ft_lookup(args, '='))
			env_export(export, new_key, ft_strdup(key[1]), 0);
		else if (!ft_lookup(args, '='))
			env_export(export, new_key, NULL, 1);
		free(new_key);
	}
	else
	{
		if (ft_lookup(args, '+'))
			ft_lstadd_back(export, ft_lstnew(new_key, ft_strdup(key[1]), 0));
		else if (ft_lookup(args, '='))
			ft_lstadd_back(export, ft_lstnew(new_key, ft_strdup(key[1]), 0));
		else if (!ft_lookup(args, '='))
			ft_lstadd_back(export, ft_lstnew(new_key, NULL, 1));
	}
	fr_args(key);
}

bool	my_export(t_commands *cmnds, t_shell *shell, t_env **env, int flag)
{
	t_env		**export;
	t_commands	*curr;
	int			i;

	i = 1;
	curr = cmnds;
	export = env;
	if (curr->redirect && !flag && handle_redirections(curr->redirect))
		return (shell->exit_status = EXIT_FAILURE, true);
	if (!curr->args[i])
		return (sort_export(export), true);
	while (curr->args[i])
	{
		if (!is_valid_export(curr->args[i]))
		{
			ft_fprintf(2, "minishell: export: `%s': not a valid identifier\n",
				curr->args[i]);
			return (shell->exit_status = EXIT_FAILURE, true);
		}
		export_handler(export, curr->args[i]);
		i++;
	}
	env = export;
	return (shell->exit_status = EXIT_SUCCESS, true);
}
