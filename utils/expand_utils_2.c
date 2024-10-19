/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 05:41:29 by hel-asli          #+#    #+#             */
/*   Updated: 2024/10/19 06:03:35 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

void gar_protect(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (is_rev_special(str[i]))
			match_rev(str, i);
		i++;
	}
}

void protect_tab(char **tab)
{
	int i;

	i = 0;
	while (tab[i])
	{
		gar_protect(tab[i]);
		i++;
	}
}
