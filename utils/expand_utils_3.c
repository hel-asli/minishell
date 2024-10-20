/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils_3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 05:47:06 by hel-asli          #+#    #+#             */
/*   Updated: 2024/10/20 23:27:40 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

char	*get_from_env(t_shell *shell, char *arg, int *i)
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

char	*get_new_value(t_shell *shell, char *arg, int *i)
{
	char	*new_value;

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

char	**replace_tab(char **tab, char *arg, t_shell *shell)
{
	char	*new_arg;
	char	**sp;

	(1) && (new_arg = NULL, sp = NULL);
	space_to_gar(arg);
	new_arg = expand_arg(arg, shell);
	if (new_arg)
	{
		if (check_var(arg) && ft_strchr(new_arg, ' '))
		{
			space_to_gar(new_arg);
			sp = ft_split(new_arg);
			free(new_arg);
			tab = re_build_arg(tab, sp);
			fr_args(sp);
		}
		else
		{
			gar_protect(new_arg);
			tab = add_arr(tab, new_arg);
			free(new_arg);
		}
	}
	return (tab);
}
