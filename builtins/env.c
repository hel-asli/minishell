/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 16:38:37 by oel-feng          #+#    #+#             */
/*   Updated: 2024/10/11 22:23:50 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	my_env(t_env **env)
{
	t_env	*curr;

	curr = *env;
	
	while (curr)
	{
		// printf("%d -> ", curr->exported);
		if (curr->value != NULL)
			printf("%s=%s\n", curr->key, curr->value);
		else if (curr->exported != 0)
			printf("%s=\n", curr->key);
		curr = curr->next;
	}
	return (true);
}
