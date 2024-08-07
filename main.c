/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-feng <oel-feng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 00:20:44 by hel-asli          #+#    #+#             */
/*   Updated: 2024/08/07 03:49:57 by oel-feng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

void built_env(t_env **env, char **ev)
{
	char **sp;
	(void)env;
	int i = 0;

	while (ev[i])
	{
		sp = ft_env_split(ev[i]);
		ft_lstadd_back(env, ft_lstnew(sp[0], sp[1]));
		free(sp);
		i++;
	}
}
void print_env(t_env *env)
{
	// t_env *cur = env;
	while (env)
	{
		printf("-----------------\n");
		printf("%s=", env->key);
		printf("%s\n", env->value);
		env = env->next;
	}
}

void	set_env(t_env **env)
{
	char *pwd = getcwd(NULL, 0);
	ft_lstadd_back(env, ft_lstnew(ft_strdup("PWD"), pwd));
	ft_lstadd_back(env, ft_lstnew(ft_strdup("SHLVL"), ft_strdup("1")));
	ft_lstadd_back(env, ft_lstnew(ft_strdup("_"), ft_strdup("/usr/bin/env")));
	ft_lstadd_back(env, ft_lstnew(ft_strdup("PATH"), ft_strdup("/usr/gnu/bin:/usr/local/bin:/bin:/usr/bin:.")));
}

int	main(int ac, char **av, char **ev)
{
	(void)av;
	t_parsing parsing;
	t_env 	*env;
	if (ac != 1)
		exit(EXIT_FAILURE);
	env = NULL;
	if (ev == NULL || *ev == NULL)
		set_env(&env);
	else
		built_env(&env, ev);
	print_env(env);
	env_clear(&env);
	read_input(&parsing, "minishell : ");
}
