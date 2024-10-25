/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 00:20:44 by hel-asli          #+#    #+#             */
/*   Updated: 2024/10/25 04:47:07 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_rl_signal = 0;

static void	print_zsh_msg(void)
{
	ft_fprintf(1, "\nThe default interactive shell is now zsh.\n");
	ft_fprintf(1, "To update your account to use zsh,");
	ft_fprintf(1, " please run `chsh -s /bin/zsh`.\n");
	ft_fprintf(1, "For more details, please visit ");
	ft_fprintf(1, "https://t.ly/uuCAq.\n");
	return ;
}

void	set_terminal_new_attr(struct termios *old_attr)
{
	struct termios	new_attr;

	tcgetattr(STDIN_FILENO, &new_attr);
	new_attr = *old_attr;
}

void	restore_terminal_old_attr(struct termios *old_attr)
{
	tcsetattr(STDIN_FILENO, TCSANOW, old_attr);
}

void	setup_signals(void)
{
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, sigint_handler);
}

int	main(int ac, char **av, char **ev)
{
	t_shell		shell;

	(void)av;
	if (!isatty(STDIN_FILENO) || ac != 1)
		exit(EXIT_FAILURE);
	rl_catch_signals = 0;
	setup_signals();
	shell.env = NULL;
	shell.commands = NULL;
	shell.exit_status = 0;
	shell.escape = 1;
	if (!ev || !*ev)
		set_env(&shell.env);
	else
		built_env(&shell.env, ev);
	print_zsh_msg();
	read_input(&shell, "minishell$ ");
	env_clear(&shell.env);
}
