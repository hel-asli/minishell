/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 02:46:47 by oel-feng          #+#    #+#             */
/*   Updated: 2024/10/18 11:18:39 by hel-asli         ###   ########.fr       */
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
		if (is_redirection(args[i]) ||  !ft_strcmp(args[i], "<<"))
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
	if (!tab)
		return (0);
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
		{
			if (!env->value)
				return (NULL);
			return (ft_strdup(env->value));
		}
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
bool is_valid(char c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (true);
	else if ((c >= '0' && c <= '9') || c == '_' || c == '$')
		return (true);

	return (false);
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

char *get_from_env(t_shell *shell, char *arg, int *i)
{
	int		j;
	char	*new_value;
	char	*env_value;
	char	*key;

	(1) && (j = *i, new_value = ft_strdup(""), key = NULL, env_value = NULL);
	while (arg[(*i)] && is_valid(arg[*i]) && arg[(*i)] != '$')
		(*i)++;
	if (j == *i)
		new_value = str_add_char(new_value, '$');
	else
	{
		key = ft_strndup(&arg[j], *i - j);
		env_value = get_env(key, shell->env);
		if (!env_value) 
			env_value = ft_strdup("");
		new_value = ft_strjoin(new_value, env_value);
		free(key);
	}
	return (new_value);
}

char *get_new_value(t_shell *shell, char *arg, int *i)
{
	char *new_value;
	new_value = ft_strdup("");

	if (arg[(*i)] == '$')
	{
		new_value = ft_strjoin(new_value, ft_strdup("1337"));
		(*i)++;
	}
	else if (arg[*i] >= '0' && arg[*i] <= '9')
	{
		if (arg[*i] == '0')
			(1) && (free(new_value), ft_strdup("minishell"));
		else
			(1) && (free(new_value), ft_strdup(""));
		(*i)++;
	}
	else if (arg[(*i)] == '?')
	{
		new_value = ft_strjoin(new_value, ft_itoa(shell->exit_status));
		(*i)++;
	}
	else
		(1) && (free(new_value), new_value = get_from_env(shell, arg, i));
	return (new_value);
}

void quotes_check(char c, bool *in_single, bool *in_double, int *i)
{
		if (c == '\'' && !(*in_double))
		{
			(*in_single) = !(*in_single);
			(*i)++;
		}
		else if (c == '"' && !(*in_single))
		{
			(*in_double) = !(*in_double);
			(*i)++;
		}	
}

char *expand_arg(char *arg, t_env *env, t_shell *shell, char *env_key)
{
	char	*new_value;
	char	*env_value;
	bool	quotes[2];
	int		i;

	(1) && (i = 0, quotes[0] = false, quotes[1] = false, env_key = NULL, env_value = NULL);
	new_value = ft_strdup("");
	if (!new_value)
		err_handle("allocation");
	while (arg[i])
	{
		if (arg[i] == '\'' || arg[i] == '"')
			quotes_check(arg[i], &quotes[0], &quotes[1], &i);
		else if (arg[i] == '$' && !quotes[0])
		{
			i++;
			if ((arg[i] == '"' || arg[i] == '\'') && !quotes[1] && !quotes[0])
				continue ;
			new_value = ft_strjoin(new_value, get_new_value(shell, arg, &i));
		}
		else
			(1) && (new_value = str_add_char(new_value, arg[i]), i++);
	}
	if (!new_value[0])
		return (free(new_value), NULL);
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
    while (args && args[j])
    {
        ptr[j] = ft_strdup(args[j]);
        j++;
    }
    while (sp[k])
    {
		gar_protect(sp[k]);
     	ptr[j++] = ft_strdup(sp[k]);
        k++;
    }
    ptr[j] = NULL;
	fr_args(args);
    return (ptr);
}

void fr_args(char **args)
{
	int i = -1;

	if (args)
	{
		while (args[++i])
			free(args[i]);
		free(args);
	}
}

char **add_arr(char **args, char *str)
{
	size_t len;

	len = arr_len(args);
	char **new = malloc((sizeof(char *) * (len + 2)));
	if (!new)
		return (NULL);
	size_t i = 0;
	while (i < len && args)
	{
		new[i] = ft_strdup(args[i]);
		i++;
	}
	if (str)
		new[i++] = ft_strdup(str);
	new[i] = NULL;
	fr_args(args);
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

void match_rev(char *line, int i)
{
	if (line)
	{
		if (line[i] == 1)
			line[i] = '|';
		else if (line[i] == 2)
			line[i] = '<';
		else if (line[i] ==  3)
			line[i] = '>';
		else if (line[i] == 4)
			line[i] = 32;
		else if (line[i] >= 15 && line[i] <= 19)
			line[i] -= 6;
	}
}

void gar_protect(char *str)
{
	for (int i = 0; str[i]; i++)
	{
		if (is_rev_special(str[i]))
			match_rev(str, i);
	}
}


char **expand_args (char **args, t_shell *shell)
{
	int i = 0;
	t_shell *tmp_shell;

	tmp_shell = shell; 
	char *new_arg = NULL;
	char **sp = NULL;
	char **tab = NULL;

	while (args[i])
	{
		if (ft_strchr(args[i], '$'))
		{	
			space_to_gar(args[i]);
			new_arg = expand_arg(args[i], tmp_shell->env, shell, NULL);
			if (new_arg)
			{
				if (check_var(args[i]) && ft_strchr(new_arg, ' '))
				{
					space_to_gar(new_arg);
					sp = ft_split(new_arg);
					free(new_arg);
					tab = re_build_arg(tab, sp);
					ft_free(sp);
				}
				else
				{
					gar_protect(new_arg);
					tab = add_arr(tab, new_arg);
					free(new_arg);
				}
			}
		}
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
void expand_redirect(t_redirect *redirect, t_env *env, t_shell *shell)
{
	t_redirect *tmp;
	char	*new_file;
	
	tmp = redirect;
	new_file = NULL;
	while (tmp)
	{
		if (tmp->type != HEREDOC_INPUT)
		{
			new_file = expand_arg(tmp->file, env, shell, NULL);
			if (!new_file && !ft_strchr(tmp->file, '"') && !ft_strchr(tmp->file, '\''))
				tmp->is_ambgious = true;
			else if (new_file && check_var(tmp->file) && ft_strchr(new_file, 32))
				tmp->is_ambgious = true;
			free(tmp->file);
			tmp->file = new_file;
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

	i = 0;
	redirect = NULL;
	while (pipes[i])
	{
		tab = ft_split(pipes[i]);
		if (!tab)
			err_handle("Allocation Faile");
		for (int j = 0; tab[j] ; j++)
			gar_protect(tab[j]);
		redirect = build_redirection(tab);
		args = args_allocation(tab, count_non_redirection_arg_size(tab)); 
		ft_free(tab);
		args = expand_args(args, *shell);
		expand_redirect(redirect, (*shell)->env, *shell);
		if (!args && !redirect)
		{
			i++;
			continue;
		}
		ft_back_addlst(&(*shell)->commands, ft_newlist(args, redirect));
		i++;
	}
	ft_free(pipes);
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