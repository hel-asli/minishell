/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 17:15:23 by oel-feng          #+#    #+#             */
/*   Updated: 2024/10/11 22:12:25 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	exit_error(int flag)
{
	if (flag == 1)
		printf("exit\nminishell: exit: numeric argumet required\n");
	else if (flag == 2)
		printf("exit\nminishell: exit: too many arguments\n");
	exit(255);
}

static void	exit_check(char *str)
{
	size_t	i;
	bool	check;

	i = 0;
	check = false;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		check = true;
		if (str[i] < 48 || str[i] > 57)
			exit_error(1);
		i++;
	}
	if (!check)
		exit_error(1);
	return ;
}

bool	my_exit(t_commands *cmnds)
{
	int			i;
	long long	num;
	t_commands	*curr;

	i = 1;
	curr = cmnds;
	if (handle_redirections(cmnds->redirect) < 0)
		return (true);
	while (curr->args[i])
		i++;
	if (i == 1)
		exit(EXIT_SUCCESS);
	if (i >= 2)
		exit_check(curr->args[1]);
	if (i > 2)
		exit_error(2);
	num = ft_exit_atol(curr->args[1]);
	exit(num % 256);
	return (true);
}
