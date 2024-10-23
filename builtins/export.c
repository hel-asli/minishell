/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-feng <oel-feng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 17:55:49 by oel-feng          #+#    #+#             */
/*   Updated: 2024/10/23 04:45:50 by oel-feng         ###   ########.fr       */
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

void	export_key_exist(t_env **export, char *val, char *key, char *args)
{
	if (ft_lookup(args, '+'))
		env_concat(export, key, ft_strdup(val));
	else if (ft_lookup(args, '='))
		env_export(export, key, ft_strdup(val), 0);
	else if (!ft_lookup(args, '='))
		env_export(export, key, NULL, 1);
	free(key);
}

void	export_key_new(t_env **export, char *val, char *key, char *args)
{
	if (ft_lookup(args, '+'))
		ft_lstadd_back(export, ft_lstnew(key, ft_strdup(val), 0));
	else if (ft_lookup(args, '='))
		ft_lstadd_back(export, ft_lstnew(key, ft_strdup(val), 0));
	else if (!ft_lookup(args, '='))
		ft_lstadd_back(export, ft_lstnew(key, NULL, 1));
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
		export_key_exist(export, key[1], new_key, args);
	else
		export_key_new(export, key[1], new_key, args);
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
