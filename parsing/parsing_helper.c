/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 02:46:47 by oel-feng          #+#    #+#             */
/*   Updated: 2024/10/19 06:11:35 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


char *check_value(char *value)
{
	if (!value[0])
		return (free(value), NULL);
	return (value);
}

char *expand_arg(char *arg, t_env *env, t_shell *shell)
{
	char	*value;
	bool	quotes[2];
	int		i;

	(1) && (i = 0, quotes[0] = false, quotes[1] = false);
	value = ft_strdup("");
	if (!value)
		err_handle("allocation");
	while (arg[i])
	{
		if (arg[i] == '\'' && !quotes[1])
			(1) && (quotes[0] = !quotes[0], i++);
		else if (arg[i] == '"' && !quotes[0])
			(1) && (quotes[1] = !quotes[1], i++);
		else if (arg[i] == '$' && !quotes[0])
		{
			i++;
			if ((arg[i] == '"' || arg[i] == '\'') && !quotes[1] && !quotes[0])
				continue ;
			value = ft_strjoin(value, get_new_value(shell, arg, &i));
		}
		else
			(1) && (value = str_add_char(value, arg[i]), i++);
	}
	return (check_value(value));
}

char	**expand_args(char **args, t_shell *shell)
{
	int		i;
	char	*new_arg;
	char	**sp;
	char	**tab;

	(1) && (i = 0, new_arg = NULL, sp = NULL, tab = NULL);
	while (args[i])
	{
		if (ft_strchr(args[i], '$'))
			tab = replace_tab(tab, args[i], shell);
		else
		{
			args[i] = del_quote(args[i]);
			gar_protect(args[i]);
			tab = add_arr(tab, args[i]);
		}
		i++;
	}
	return (fr_args(args), tab);
}

void	expand_redirect(t_redirect *redirect, t_env *env, t_shell *shell)
{
	t_redirect	*tmp;
	char		*file;

	(1) && (tmp = redirect, file = NULL);
	while (tmp)
	{
		if (tmp->type != HEREDOC_INPUT)
		{
			file = expand_arg(tmp->file, env, shell);
			if (!file && !ft_strchr(tmp->file, '"')
				&& !ft_strchr(tmp->file, '\''))
				tmp->is_ambgious = true;
			else if (file && check_var(tmp->file) && ft_strchr(file, 32))
				tmp->is_ambgious = true;
			free(tmp->file);
			tmp->file = file;
		}
		else
			tmp->file = del_quote(tmp->file);
		tmp = tmp->next;
	}
}

void	process_pipe_cmds(t_shell **shell, char **pipes)
{
	int			i;
	char		**tab;
	char		**args;
	t_redirect	*redirect;

	(1) && (i = 0, redirect = NULL);
	while (pipes[i])
	{
		tab = ft_split(pipes[i]);
		if (!tab)
			err_handle("Allocation Faile");
		(1) && (protect_tab(tab), redirect = build_redirection(tab));
		args = args_allocation(tab, count_non_redirection_arg_size(tab));
		(1) && (fr_args(tab), args = expand_args(args, *shell));
		expand_redirect(redirect, (*shell)->env, *shell);
		if (!args && !redirect)
		{
			i++;
			continue ;
		}
		ft_back_addlst(&(*shell)->commands, ft_newlist(args, redirect));
		i++;
	}
	fr_args(pipes);
}

void	print_cmds(t_commands *cmds)
{
	int			i;
	t_redirect	*red;

	while (cmds)
	{
		printf("----------------------------------------------\n");
		printf("args : ");
		if (cmds->args)
		{
			i = 0;
			while (cmds->args[i])
			{
				printf("{%s} ", cmds->args[i]);
				i++;
			}
		}
		else
			printf("(None) args . ");
		printf("\n");
		red = cmds->redirect;
		printf("redirections : \n");
		if (!red)
			printf("(None)\n");
		else
		{
			while (red)
			{
				printf("type : %u\n", red->type);
				printf("expanded : %d\n", red->expanded);
				printf("is_ambigous : %d\n", red->is_ambgious);
				printf("file : {%s}\n", red->file);
				red = red->next;
			}
		}
		printf("----------------------------------------------\n");
		cmds = cmds->next;
	}
}