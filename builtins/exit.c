/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 17:15:23 by oel-feng          #+#    #+#             */
/*   Updated: 2024/10/21 03:27:10 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../minishell.h"

void	exit_error(t_shell *shell, int flag)
{
	if (flag == 1)
	{
		printf("exit\nminishell: exit: numeric argumet required\n");
		exit(255);
	}
	else if (flag == 2)
	{
		shell->exit_status = EXIT_FAILURE;
		printf("exit\nminishell: exit: too many arguments\n");
	}
}

static void	exit_check(t_shell *shell, char *str)
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
			exit_error(shell, 1);
		i++;
	}
	if (!check)
		exit_error(shell, 1);
	return ;
}

bool	my_exit(t_commands *cmnds, t_shell *shell, int flag)
{
	int			i;
	long long	num;
	t_commands	*curr;

	i = 1;
	curr = cmnds;
	if (curr->redirect && !flag && handle_redirections(curr->redirect) < 0)
		return (shell->exit_status = EXIT_FAILURE ,true);
	while (curr->args[i])
		i++;
	if (i == 1)
	{
		ft_putendl_fd("exit", 2);
		exit(EXIT_SUCCESS);
	}
	if (i >= 2)
		exit_check(shell, curr->args[1]);
	if (i > 2)
		exit_error(shell, 2);
	else
	{
		num = ft_exit_atol(shell, curr->args[1]);
		ft_putendl_fd("exit", 2);
		exit(num % 256);
	}
	return (true);
}
