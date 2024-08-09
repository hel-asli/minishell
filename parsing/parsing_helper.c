/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 02:46:47 by oel-feng          #+#    #+#             */
/*   Updated: 2024/08/09 01:31:21 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

size_t count_arg_size(char **args)
{
	int	i;
	size_t count;

	i = 0;
	count = 0;
	while (args[i])	
	{
		if (is_redirection(args[i]) || !ft_strcmp(args[i], "<<"))
			i += 2;
		else
		{
			count++;
			i++;
		}
	}

	return (count);
}
t_redirect *build_redirection(char **args)
{
	int i = 0;
	t_redirect *redirect;

	redirect = NULL;
	while (args[i])
	{
		if (args[i + 1] && (is_redirection(args[i])
				|| !ft_strcmp(args[i], "<<")))
		{
			ft_lst_add_redir(&redirect, ft_new_redir(args[i], args[i + 1]));
			i += 2;
		}
		else
			i++;
	}

	return (redirect);
}

char **args_allocation(char **tab, size_t arg_count)
{
	char	**args;
	int		i;
	size_t		k;

	args = malloc(sizeof(char *) * (arg_count + 1));
	if (!args)
		err_handle("Allocation Faile!!");
	i = 0;
	k = 0;
	while (tab[i])
	{
		if (tab[i + 1] && (is_redirection(tab[i])
				|| !ft_strcmp(tab[i], "<<")))
			i += 2;
		else
		{
			args[k++] = ft_strdup(tab[i]);
			i++;
		}
	}
	args[k] = NULL;
	return (args);
}

void	pipes_cmds(t_commands **commands, char **pipes)
{
	int			i;
	int			k;
	char		**tab;
	char		**arg;
	t_redirect	*redirect;

	k = 0;
	i = -1;
	redirect = NULL;
	while (pipes[++i])
	{
		tab = ft_split(pipes[i]);
		if (!tab)
			err_handle("Allocation Faile");
		for (int j = 0; tab[j] ; j++)
			space_to_gar(tab[j]);
		redirect = build_redirection(tab);
		arg = args_allocation(tab, count_arg_size(tab)); 
		ft_back_addlst(commands, ft_newlist(arg[0], arg, redirect));
		free(tab);
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
