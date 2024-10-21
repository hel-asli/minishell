/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 22:35:27 by oel-feng          #+#    #+#             */
/*   Updated: 2024/10/21 04:12:36 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	remove_env(t_env **env, char *key)
{
	t_env	*curr;
	t_env	*prev;

	curr = *env;
	prev = NULL;
	while (curr)
	{
		if (!ft_strcmp(curr->key, key))
		{
			if (prev)
				prev->next = curr->next;
			else
				*env = curr->next;
			free(curr->key);
			free(curr->value);
			free(curr);
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
}
bool 	unset_check(char *str)
{
	int	i;

	i = 0;
	if (!ft_isalpha(str[i]) && str[i] != '_')
		return (false);
	i++;
	while (str[i])
	{
		if (ft_isalnum(str[i]) || str[i] == '_')
			i++;
		else
			return (false);
	}
	return (true);	
}
bool	my_unset(t_commands *cmnds, t_shell *shell, t_env **env, int flag)
{
	int			i;
	t_commands	*curr;

	i = 0;
	curr = cmnds;
	if (curr->redirect && !flag && handle_redirections(curr->redirect) == -1)
	{
		shell->exit_status = EXIT_FAILURE;
		return (true);
	}
	while (curr->args[++i])
	{
		if (!unset_check(curr->args[i]))
		{
			ft_fprintf(2, "minishell: export: `%s': not a valid identifier\n", curr->args[i]);
			shell->exit_status = EXIT_FAILURE;
		}
		else
			remove_env(env, curr->args[i]);
	}
	shell->exit_status = EXIT_SUCCESS;
	return (true);
}
