/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-feng <oel-feng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 04:45:50 by oel-feng          #+#    #+#             */
/*   Updated: 2024/10/23 04:47:01 by oel-feng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	env_concat(t_env **env, char *key, char *value)
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

void	env_export(t_env **env, char *key, char *value, int exported)
{
	t_env	*curr;

	curr = *env;
	while (curr)
	{
		if (!ft_strcmp(curr->key, key))
		{
			if (value == NULL)
				return ;
			free(curr->value);
			curr->value = ft_strdup(value);
			curr->exported = exported;
			free(value);
			return ;
		}
		curr = curr->next;
	}
}
