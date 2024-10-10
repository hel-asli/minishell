/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-feng <oel-feng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 20:44:19 by oel-feng          #+#    #+#             */
/*   Updated: 2024/10/10 00:07:56 by oel-feng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int ft_env_size(t_env *env)
{
    int i;
    t_env *tmp;

    i = 0;
    tmp = env;
    while (tmp)
    {
        i++;
        tmp = tmp->next;
    }
    return (i);
}

char **list_arr(t_env *env)
{
    char **ptr;
    t_env *tmp;
    int i;
	
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
