/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-feng <oel-feng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 16:38:37 by oel-feng          #+#    #+#             */
/*   Updated: 2024/10/10 00:09:07 by oel-feng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	my_env(t_env **env)
{
	t_env	*curr;

	curr = *env;
	while (curr)
	{
		if (curr->value != NULL)
			printf("%s=%s\n", curr->key, curr->value);
		curr = curr->next;
	}
	return (true);
}
