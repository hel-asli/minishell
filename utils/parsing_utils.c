/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 18:11:55 by oel-feng          #+#    #+#             */
/*   Updated: 2024/10/19 04:08:00 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	is_special(char c)
{
	return (c == '|' || c == '<' || c == '>' || is_space(c));
}

bool	is_rev_special(char c)
{
	return (c == 1 || c == 2 || c == 3 || c == 4 || (c >= 15 && c <= 19));
}

bool	in_quotes(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
			return (true);
		i++;
	}
	return (false);
}

void	match_char(char *line, int i)
{
	if (line)
	{
		if (line[i] == '|')
			line[i] = 1;
		else if (line[i] == '<')
			line[i] = 2;
		else if (line[i] == '>')
			line[i] = 3;
		else if (line[i] == 32)
			line[i] = 4;
		else if (line[i] >= 9 && line[i] <= 13)
			line[i] += 6;
	}
}

int	check_dots(char *line)
{
	if (!strcmp(line, "."))
	{
		printf("minishell: .: filename argument required\n");
		printf(".: usage: . filename [arguments]\n");
		return (2);
	}
	else if (!strcmp(line, ".."))
	{
		printf("minishell: ..: command not found\n");
		return (127);
	}
	return (0);
}
