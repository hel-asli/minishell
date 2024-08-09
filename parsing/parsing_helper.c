/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 02:46:47 by oel-feng          #+#    #+#             */
/*   Updated: 2024/08/10 00:37:43 by hel-asli         ###   ########.fr       */
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
size_t arr_len(char **tab)
{
	size_t i;

	i = 0;
	while (tab[i])
		i++;
	return (i);
}

bool ft_strchr(char *str, char c)
{
	int i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (true);
		i++;
	}
	return (false);
}

char *get_env(char *key, t_env *env)
{
	t_env *tmp;

	tmp = env;
	while (env)
	{
		if (!ft_strcmp(env->key, key))
			return (env->value);
		env = env->next;
	}

	return (NULL);
}


void	print_env(t_env *env)
{
	while (env)
	{
		printf("-----------------\n");
		printf("%s=", env->key);
		printf("%s\n", env->value);
		env = env->next;
	}
}

char **ft_realloc(char **args, char **tab, int index)
{
	size_t args_len = arr_len(args) - 1;
	size_t tab_len = arr_len(tab);
	int i = 0;
	int j = 0;
	int k = 0;
	char **arr = malloc(sizeof(char **) * (args_len + tab_len + 1));
	if (!arr)
		err_handle("Allocation Fail");
	
	while (args[k])
	{
		if (k != index)
		{
			arr[i] = ft_strdup(args[k]);
			i++;
		}
		k++;
	}
	while (tab[j])
	{
		arr[i] = ft_strdup(tab[j]);
		i++;
		j++;
	}

	arr[i] = NULL;
	ft_free(args);
	ft_free(tab);
	return (arr);
}

char **expand_args(char **args, t_env *env)
{
	// size_t len = arr_len(args);
	int i = 0;
	int j = 0;
	int k = 0;
	int n = 0;
	int o = 0;
	char *env_key = NULL;
	char *env_value = NULL;
	while (args[i])
	{
		if (ft_strchr(args[i], '$'))
		{
			while (args[i][j] && args[i][j] != '$')
				j++;
			if (args[i][j] == '$')
				j++;
			o = j;
			while (args[i][j] && args[i][j] != ' ' && args[i][j] != '\t')
			{
				j++;
				k++;
			}
			env_key = malloc(sizeof(char) * (k  + 1));
			if (!env_key)
				err_handle("Allocation Fail");
			while (args[i][o] && args[i][o] != ' ' && args[i][o] != '\t')
			{
				env_key[n] = args[i][o];
				n++;
				o++;
			}
			env_key[n] = 0;
			env_value = get_env(env_key, env);
			if (count_words(env_value) == 1)
			{
				free(args[i]);
				args[i] = env_value;
			}
			else if (count_words(env_value) > 1)
			{
				printf("i : %d\n", i);
				args = ft_realloc(args, ft_split(env_value), i);
			}
		}
		i++;
	}
	return (args);
}
void	pipes_cmds(t_shell **shell, char **pipes)
{
	int			i;
	int			k;
	char		**tab;
	char		**args;
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
		args = args_allocation(tab, count_arg_size(tab)); 

		args = expand_args(args, (*shell)->env);
		ft_back_addlst(&(*shell)->commands, ft_newlist(args[0], args, redirect));
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
