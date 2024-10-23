/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 00:20:44 by hel-asli          #+#    #+#             */
/*   Updated: 2024/10/23 04:40:33 by hel-asli         ###   ########.fr       */
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
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = sigint_handler;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_RESETHAND | SA_RESTART;
	sigaction(SIGINT, &sa_int, NULL);
	sa_quit.sa_handler = SIG_IGN;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = SA_RESTART;
	sigaction(SIGQUIT, &sa_quit, NULL);
}

int	main(int ac, char **av, char **ev)
{
	t_shell		shell;

	(void)av;
	if (!isatty(STDIN_FILENO) || ac != 1)
		exit(EXIT_FAILURE);
	tcgetattr(STDIN_FILENO, &shell.old_attr);
	tcgetattr(STDIN_FILENO, &shell.copy);
	shell.copy.c_lflag &= ~(ECHOCTL);
	tcsetattr(STDIN_FILENO, TCSANOW, &shell.copy);
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
	restore_terminal_old_attr(&shell.old_attr);
}
