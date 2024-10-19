/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 03:48:28 by hel-asli          #+#    #+#             */
/*   Updated: 2024/10/19 06:17:41 by hel-asli         ###   ########.fr       */
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
	if (rl_signal)
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
	if (!rl_signal)
		printf("Quit: 3\n");
}
