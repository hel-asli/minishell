/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-feng <oel-feng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 00:20:44 by hel-asli          #+#    #+#             */
/*   Updated: 2024/10/10 17:47:16 by oel-feng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	built_env(t_env **env, char **ev)
{
	char	**sp;
	int		i;

	i = 0;
	(void)env;
	while (ev[i])
	{
		sp = ft_env_split(ev[i]);
		ft_lstadd_back(env, ft_lstnew(ft_strdup(sp[0]), ft_strdup(sp[1])));
		ft_free(sp);
		i++;
	}
}


void	set_env(t_env **env)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	ft_lstadd_back(env, ft_lstnew(ft_strdup("PATH"),
			ft_strdup("/usr/gnu/bin:/usr/local/bin:/bin:/usr/bin:.")));
	ft_lstadd_back(env, ft_lstnew(ft_strdup("PWD"), pwd));
	ft_lstadd_back(env, ft_lstnew(ft_strdup("SHLVL"), ft_strdup("1")));
	ft_lstadd_back(env, ft_lstnew(ft_strdup("_"), ft_strdup("/usr/bin/env")));
	ft_lstadd_back(env, ft_lstnew(ft_strdup("OLDPWD"), NULL));
}

int	main(int ac, char **av, char **ev)
{
	t_shell		shell;

	(void)av;
	if (ac != 1)
		exit(EXIT_FAILURE);
	shell.env = NULL;
	shell.commands = NULL;
	shell.exit_status = -1;
	if (ev == NULL || *ev == NULL)
		set_env(&shell.env);
	else
		built_env(&shell.env, ev);
	read_input(&shell, "minishell: ");
}
