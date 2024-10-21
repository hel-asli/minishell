/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-feng <oel-feng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 05:38:22 by hel-asli          #+#    #+#             */
/*   Updated: 2024/10/21 21:25:13 by oel-feng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

size_t	count_non_redirection_arg_size(char **args)
{
	int		i;
	size_t	count;

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

size_t	arr_len(char **tab)
{
	size_t	i;

	i = 0;
	if (!tab)
		return (0);
	while (tab[i])
		i++;
	return (i);
}

char	*get_env(char *key, t_env *env)
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

char	*str_add_char(char *str, char c)
{
	int		i;
	char	*ptr;

	i = 0;
	ptr = malloc(sizeof(char) * (ft_strlen(str) + 2));
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

char	**re_build_arg(char **args, char **sp)
{
	size_t	len;
	int		j;
	int		k;
	char	**ptr;

	j = 0;
	k = 0;
	len = arr_len(args) + arr_len(sp);
	ptr = malloc(sizeof(char *) * (len + 1));
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
