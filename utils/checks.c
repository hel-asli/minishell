/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-feng <oel-feng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 18:24:57 by oel-feng          #+#    #+#             */
/*   Updated: 2024/10/18 18:30:24 by oel-feng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	check_file(char *file)
{
	int		i;
	bool	test;

	i = 0;
	test = false;
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

void	match_rev(char *line, int i)
{
	if (line)
	{
		if (line[i] == 1)
			line[i] = '|';
		else if (line[i] == 2)
			line[i] = '<';
		else if (line[i] == 3)
			line[i] = '>';
		else if (line[i] == 4)
			line[i] = 32;
		else if (line[i] >= 15 && line[i] <= 19)
			line[i] -= 6;
	}
}

bool	check_var(char *arg)
{
	int		i;
	bool	in_quotes;

	i = 0;
	in_quotes = false;
	while (arg[i])
	{
		if (arg[i] == '"')
			in_quotes = !in_quotes;
		if (!in_quotes && arg[i] == '$' && is_valid(arg[i + 1]))
			return (true);
		i++;
	}
	return (false);
}

bool	is_valid(char c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (true);
	else if ((c >= '0' && c <= '9') || c == '_' || c == '$')
		return (true);
	return (false);
}

bool	ft_strchr(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (true);
		i++;
	}
	return (false);
}
