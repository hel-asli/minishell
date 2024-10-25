/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_help.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 03:32:22 by oel-feng          #+#    #+#             */
/*   Updated: 2024/10/25 02:25:51 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	starts_with(char *start, char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (1);
	while (str[i] && start[i])
	{
		if (str[i] == start[i])
			i++;
		else
			break ;
	}
	if (!str[i])
		return (0);
	return (1);
}

bool	is_not_sub(const char *str, const char *pwd)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (str[i] && pwd[j] && str[i] == pwd[j])
	{
		i++;
		j++;
	}
	if (!pwd[j] && !str[i])
		return (true);
	if (!pwd[j] && str[i] == '/')
		i++;
	while (str[i])
	{
		if (str[i] == '/' && !str[i + 1])
			return (false);
		else if (str[i] == '/' && str[i + 1] != '/')
			return (true);
		i++;
	}
	return (false);
}

char	*wildcard_dir(struct dirent *entity, char *str)
{
	char	**sp;
	char	*new_str;

	sp = ft_split_v2(str, '/');
	new_str = NULL;
	if (entity->d_type == DT_DIR && check_pattern(sp[arr_len(sp) - 1],
			entity->d_name))
	{
		new_str = str_add_char(ft_strdup(entity->d_name), '/');
	}
	fr_args(sp);
	return (new_str);
}

void	file_change(t_redirect *tmp, char *file)
{
	if (!file)
	{
		free(tmp->file);
		tmp->file = ft_strdup("");
	}
	else
	{
		free(tmp->file);
		tmp->file = del_quote(file);
	}
}
