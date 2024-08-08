/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printer.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-feng <oel-feng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 02:46:47 by oel-feng          #+#    #+#             */
/*   Updated: 2024/08/08 03:09:48 by oel-feng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	pipes_cmds(t_commands **commands, char **pipes)
{
	int			i;
	int			j;
	int			k;
	char		**tab;
	char		**arg;
	int			arg_count;
	t_redirect	*redirect;

	i = 0;
	j = 0;
	k = 0;
	arg_count = 0;
	redirect = NULL;
	while (pipes[i])
	{
		printf("%d -- > %s\n", i, pipes[i]);
		tab = ft_split(pipes[i]);
		if (!tab)
			puts("ook");
		j = 0;
		printf("j : %d\n", j);
		arg_count = 0;
		printf("arg_count : %d\n", arg_count);
		while (tab[j])
		{
			if (tab[j + 1] && (is_redirection(tab[j])
					|| !ft_strcmp(tab[j], "<<")))
			{
				ft_lst_add_redir(&redirect, ft_new_redir(tab[j], tab[j + 1]));
				j += 2;
			}
			else
			{
				arg_count++;
				j++;
			}
		}
		printf("--> %d\n", arg_count);
		arg = malloc(sizeof(char *) * (arg_count + 1));
		if (!arg)
			return ;
		j = 0;
		k = 0;
		printf("--- %d\n", j);
		while (tab[j])
		{
			if (tab[j + 1] && (is_redirection(tab[j])
					|| !ft_strcmp(tab[j], "<<")))
				j += 2;
			else
			{
				puts("iter");
				arg[k++] = ft_strdup(tab[j++]);
			}
		}
		arg[k] = NULL;
		ft_back_addlst(commands, ft_newlist(arg[0], arg, redirect));
		free(tab);
		i++;
	}
}

void	print_cmds(t_commands *cmds)
{
	int			i;
	t_redirect	*red;

	while (cmds)
	{
		printf("----------------------------------------------\n");
		printf("cmd : %s\n", cmds->cmd);
		printf("args : ");
		if (cmds->args)
		{
			i = 0;
			while (cmds->args[i])
			{
				printf("%s ", cmds->args[i]);
				i++;
			}
		}
		else
			printf("(None) args . ");
		printf("\n");
		red = cmds->redirect;
		printf("redirections : \n");
		if (!red)
			printf("(None) redirection . \n");
		else
		{
			while (red)
			{
				printf("type : %s\n", red->type);
				printf("file : %s\n", red->file);
				red = red->next;
			}
		}
		printf("----------------------------------------------\n");
		cmds = cmds->next;
	}
}
