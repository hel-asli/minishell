/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-feng <oel-feng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 03:48:28 by hel-asli          #+#    #+#             */
/*   Updated: 2024/10/23 03:34:36 by oel-feng         ###   ########.fr       */
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
		g_rl_signal = 3;
	}
}

void	sigquit_handler(int nb)
{
	(void)nb;
	if (!g_rl_signal)
		printf("Quit: 3\n");
}

int	signal_status(t_shell *shell)
{
	if (g_rl_signal == 3)
	{
		shell->exit_status = 1;
		g_rl_signal = 1;
	}
	if (!shell->parsing.line)
	{
		cmds_clear(&shell->commands);
		printf("exit\n");
		return (0);
	}
	return (1);
}
