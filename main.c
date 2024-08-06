/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 00:20:44 by hel-asli          #+#    #+#             */
/*   Updated: 2024/08/05 23:52:57 by hel-asli         ###   ########.fr       */
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
		ft_lstadd_back(env, ft_lstnew(sp));
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
		printf("key : %s\n", env->key);
		printf("value : %s\n", env->value);
		env = env->next;
	}
}

int	main(int ac, char **av, char **ev)
{
	(void)av;
	t_parsing parsing;
	t_env 	*env;
	if (ac != 1)
		exit(EXIT_FAILURE);

	env = NULL;
	built_env(&env, ev);
	print_env(env);
	env_clear(&env);
	read_input(&parsing, "minishell : ");
}
