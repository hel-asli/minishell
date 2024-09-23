/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 02:46:47 by oel-feng          #+#    #+#             */
/*   Updated: 2024/09/13 23:50:26y hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

size_t count_non_redirection_arg_size(char **args)
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
		if (args[i + 1] && (is_redirection(args[i]) ||  !ft_strcmp(args[i], "<<")))
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
	while (env)
	{
		if (!ft_strcmp(env->key, key))
			return (ft_strdup(env->value));
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

char *str_add_char(char *str, char c)
{
	int i = 0;
	char *ptr = malloc(sizeof(char) * (ft_strlen(str) + 2));
	if (!ptr)
		return (NULL);
	while (str[i])
	{
		ptr[i] = str[i];
		i++;
	}
	ptr[i++] = c;
	ptr[i] = '\0';
	free(str);
	return (ptr);
}

bool is_valid(char c)
{
	if ((c >= 'a' && c < 'z') || (c >= 'A' && c <= 'Z'))
		return (true);
	else if ((c >= '0' && c <= '9') || c == '_' || c == '$')
		return (true);

	return (false);
}

char *expand_arg(char *arg, t_env *env)
{
	char *new_value = ft_strdup("");
	bool in_single = false;
	bool in_double = false;
	char *env_key = NULL;
	char *env_value = NULL;
	if (!new_value)
		err_handle("allocation");
	int j = 0;
	int i = 0;
	while (arg[i])
	{
		if (arg[i] == '\'' && !in_double)
		{
			in_single = !in_single; 
			i++;
		}
		else if (arg[i] == '"' && !in_single)
		{
			in_double = !in_double;
			i++;
		}
		else if (arg[i] == '$' && !in_single)
		{
			i++;
			if ((arg[i] == '"' || arg[i] == '\'') && !in_double && !in_single)
				continue;
			if (arg[i] == '$')
			{
				new_value = ft_strjoin(new_value, ft_strdup("1337"));
				i++;
			}
			else
			{
				j = i;
				while (arg[i] && is_valid(arg[i]) && arg[i] != '$')
					i++;
				if (i == j)
					new_value = str_add_char(new_value, '$');
				else
				{
					env_key = ft_strndup(&arg[j], i - j);
					env_value = get_env(env_key, env);
					if (!env_value)
						env_value = ft_strdup("");
					new_value = ft_strjoin(new_value, env_value);
					free(env_key);
				}
			}
		}
		else
		{
			new_value = str_add_char(new_value, arg[i]);
			i++;	
		}
	}
	if (new_value[0] == 0)
		return (NULL);
	return (new_value);
}

bool check_var(char *arg)
{
	int i = 0;
	bool in_quotes = false;

	while (arg[i])
	{
		if (arg[i] == '"')
			in_quotes = !in_quotes;
		if (!in_quotes && arg[i] == '$' && is_valid(arg[i + 1]))	
			return true;
		i++;
	}
	return (false);
}

char ** re_build_arg(char **args, char **sp)
{
	size_t len;
    len = arr_len(args) + arr_len(sp);
    int j = 0;
    int k = 0;
    char **ptr = malloc(sizeof(char *) * (len + 1)); 
    if (!ptr)
        err_handle("malloc");
    while (args[j])
    {
        ptr[j] = args[j];
        j++;
    }
    while (sp[k])
    {
		gar_protect(sp[k]);
     	ptr[j++] = ft_strdup(sp[k]);
        k++;
    }
    ptr[j] = NULL;
    return (ptr);
}

char **add_arr(char **args, char *str)
{
	size_t len;

	if (!args)
		len = 0;
	else
		len = arr_len(args);
	char **new = malloc((sizeof(char *) * (len + 2)));
	if (!new)
		return (NULL);
	size_t i = 0;
	while (i < len)
	{
		new[i] = ft_strdup(args[i]);
		i++;
	}
	if (str)
		new[i++] = ft_strdup(str);
	new[i] = NULL;
	return (new);
}

void save_quotes(char *str)
{
	int i = 0;
	char dbl;
	char sgl; 

	dbl = '"' * -1;
	sgl = '\'' * -1;
	while (str[i])
	{
		if (str[i] == '"' || str[i] == '\'')
			str[i] *= -1;
		else if (str[i] == dbl || str[i] == sgl)
			str[i] *= -1;
		i++;
	}
}

void gar_protect(char *str)
{
	for (int i = 0; str[i]; i++)
	{
		if (str[i] > -128 && str[i] < 0 &&  is_rev_special(str[i]))
		{
			printf("%c\n", str[i]);
			str[i] *= -1;
		}
	}
}

char **expand_args (char **args, t_env *env)
{
	int i = 0;
	char *new_arg = NULL;
	char **sp = NULL;
	char **tab = malloc(sizeof(char *) * 1);
	tab[0] = NULL;
	while (args[i])
	{
		if (ft_strchr(args[i], '$'))
		{	
			space_to_gar(args[i]);
			printf("--> %s\n", args[i]);
			new_arg = expand_arg(args[i], env);
			printf("new_arg: %s\n", new_arg);
			if (!new_arg)
			{
				i++;
				continue;
			}
			if (check_var(args[i]) && ft_strchr(new_arg, ' '))
			{
				space_to_gar(new_arg);
				sp = ft_split(new_arg);
				tab = re_build_arg(tab, sp);
				ft_free(sp);
			}
			else
			{
				printf("hello : %s\n", new_arg);
				// space_to_gar(new_arg);
				gar_protect(new_arg);
				printf("world : %s\n", new_arg);
				// puts("ook");
				tab = add_arr(tab, new_arg);
			}
		}
		else
		{
			// space_to_gar(args[i]);
			printf("kkk: %s\n", args[i]);
			args[i] = del_quote(args[i]);
			gar_protect(args[i]);
			tab = add_arr(tab, args[i]);
		}
		i++;
	}
	return (tab);
}

bool check_file(char *file)
{
	bool test = false;
	int i = 0;

	while (file[i])
	{
		if (file[i] == '\'')
			test = !test;
		if (!test && file[i] == '$' && is_valid(file[i + 1]))
			return (true);
		i++;
	}
	return (false);
}

void expand_redirect(t_redirect *redirect, t_env *env)
{
	t_redirect *tmp;
	
	tmp = redirect;

	while (tmp)
	{
		if (tmp->type != HEREDOC_INPUT)
		{
			if (check_file(tmp->file))
			{
					tmp->file = expand_arg(tmp->file, env);
			}
		}
		tmp = tmp->next;
	}
}


void	process_pipe_cmds(t_shell **shell, char **pipes)
{
	int			i;
	char		**tab;
	char		**args;
	t_redirect	*redirect;

	i = -1;
	redirect = NULL;
	while (pipes[++i])
	{
		printf("---> %s\n", pipes[i]);
		tab = ft_split(pipes[i]);
		if (!tab)
			err_handle("Allocation Faile");
		for (int j = 0; tab[j] ; j++)
		{
			gar_protect(tab[j]);
			printf("hereeee : %s\n", tab[j]);
		}
		redirect = build_redirection(tab);
		args = args_allocation(tab, count_non_redirection_arg_size(tab)); 
		args = expand_args(args, (*shell)->env);
		expand_redirect(redirect, (*shell)->env);
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
		if (!cmds->cmd)
			printf("NULL\n");
		printf("cmd : {%s}\n", cmds->cmd);
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
				printf("file : %s\n", red->file);
				red = red->next;
			}
		}
		printf("----------------------------------------------\n");
		cmds = cmds->next;
	}
}