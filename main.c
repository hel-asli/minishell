/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 00:20:44 by hel-asli          #+#    #+#             */
/*   Updated: 2024/10/16 00:21:47 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int rl_signal = 0;

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

void sigint_handler(int nb)
{
	(void)nb;
	// if (rl_signal == 2)
	// {
	// 	// rl_signal = 3;
	// 	// ioctl(STDIN_FILENO, TIOCSTI, "\n");
	// 	// write(1, "\n", 1);
	// 	exit(0);
	// }
	if (rl_signal)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void sigquit_handler(int nb)
{
	(void)nb;
	// if (rl_signal == 1)
	// 	rl_redisplay();
	if (!rl_signal)
	{
		printf("Quit: 3\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}


void set_terminal_new_attr(struct termios *old_attr)
{
	struct termios new_attr;

	tcgetattr(STDIN_FILENO, &new_attr);
	new_attr = *old_attr;
}

void restore_terminal_old_attr(struct termios *old_attr)
{
	tcsetattr(STDIN_FILENO, TCSANOW, old_attr);
}
void setup_signals(void)
{
    struct sigaction sa_int, sa_quit;
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
	shell.exit_status = -1;
	if (ev == NULL || *ev == NULL)
		set_env(&shell.env);
	else
		built_env(&shell.env, ev);
	read_input(&shell, "minishell: ");
	restore_terminal_old_attr(&shell.old_attr);
}