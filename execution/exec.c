/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 00:49:12 by oel-feng          #+#    #+#             */
/*   Updated: 2024/10/21 13:49:22 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


char *path_handler(char *path, char *cmd)
{
	char **sp;
	int i = 0;
	char *full_path;

	sp = ft_split_v2(path, ':');
	if (!sp)
		err_handle("split");
	full_path = NULL;	
	while (sp[i])
	{
		full_path = NULL;
		full_path = ft_strjoin_char(sp[i], cmd, '/');
		if (!access(full_path, X_OK | F_OK))
			return (fr_args(sp), full_path);
		free(full_path);
		i++;
	}
	return (fr_args(sp), NULL);
}
char *find_command(char *cmd, t_env *env)
{
    char *full_path;
	int k;
	t_env *tmp;
	char **pt;

	(1) && (tmp = env, pt = NULL, k = 0, full_path = NULL);
    if (!cmd || !cmd[0])
        return NULL;
    if (my_strchr_v2(cmd, '/') != NULL)
	{
        if (access(cmd, F_OK | X_OK) == 0)
            return (ft_strdup(cmd));
        else
            return (NULL);
    }
	while (tmp)
	{	
		if (!ft_strcmp(tmp->key, "PATH"))
			full_path = path_handler(tmp->value, cmd);
		tmp = tmp->next;
	}
    return (full_path);
}
