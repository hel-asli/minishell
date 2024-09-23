/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Wayde <Wayde@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 23:14:16 by oel-feng          #+#    #+#             */
/*   Updated: 2024/09/04 04:32:43 by Wayde            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	my_cd(t_commands **cmnds, t_env **env)
{
	t_env		*tmp;
	t_commands	*curr;

	tmp = *env;
	curr = *cmnds;
	if (curr->args[1] == NULL)
	{
		if (chdir(get_env("HOME", tmp)) == -1)
			printf("minishell: cd: HOME not set\n");
	}
	else if (chdir(curr->args[1]) == -1)
	{
		printf("minishell: cd: %s: No such file or directory\n", curr->args[1]);
		return (false);
	}
	return (true);
}
