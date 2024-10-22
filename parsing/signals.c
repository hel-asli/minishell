/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 03:48:28 by hel-asli          #+#    #+#             */
/*   Updated: 2024/10/22 11:00:49 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	sigint_heredoc_handler(int nb)
{
	if (nb == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		exit(1);
	}
}

void	setup_heredoc_signals(void)
{
	signal(SIGINT, sigint_heredoc_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	sigint_handler(int nb)
{
	(void)nb;
	if (g_rl_signal)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	sigquit_handler(int nb)
{
	(void)nb;
	if (!g_rl_signal)
		printf("Quit: 3\n");
}

int	starts_with(char *start, char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (1);
	while (str[i] && start[i])
	{
		if (str[i] == start[i])
			i++;
		else
			break ;
	}
	if (!str[i])
		return (0);
	return (1);
}
