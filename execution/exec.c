/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-feng <oel-feng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 00:49:12 by oel-feng          #+#    #+#             */
/*   Updated: 2024/10/21 05:05:14 by oel-feng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char *find_command(char *cmd, t_env *env)
{
    char *full_path;
	int k;
	t_env *tmp;
	char **pt;

	tmp = env;
    if (!cmd || !cmd[0])
        return NULL;
    if (my_strchr_v2(cmd, '/') != NULL)
	{
        if (access(cmd, F_OK | X_OK) == 0)
            return ft_strdup(cmd);
        else
            return (NULL);
    }
	pt = NULL;
	k = 0;
	full_path = NULL;
	while (tmp)
	{	
		if (!ft_strcmp(tmp->key, "PATH"))
		{
			pt = ft_split_v2(tmp->value, ':');
			if (!pt)
				err_handle("split");
			while (pt[k])
			{
				full_path = NULL;
				full_path = ft_strjoin_char(pt[k], cmd, '/');
				if (access(full_path, X_OK) == 0)
					return (ft_free(pt), full_path);
				free(full_path);
				k++;
			}
			ft_free(pt);
		}
		tmp = tmp->next;
	}
    return NULL;
}
