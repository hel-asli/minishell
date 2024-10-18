/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 19:04:16 by oel-feng          #+#    #+#             */
/*   Updated: 2024/10/18 06:22:23 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static bool	ft_opt_check(char *str, char c)
{
	size_t	i;

	i = 0;
	if (str[i] == '-')
		i++;
	else
		return (false);
	while (str[i] && str[i] == c)
		i++;
	return (i == ft_strlen(str));
}

bool	my_echo(t_commands *cmnds)
{
	int			i;
	bool		opt;
	bool		wrote;
	t_commands	*curr;

	i = 0;
	opt = false;
	wrote = false;
	curr = cmnds;
	while (curr->args[++i])
	{
		if (ft_opt_check(curr->args[i], 'n') && !wrote)
			opt = true;
		else
		{
			printf("%s", curr->args[i]);
			wrote = true;
			if (curr->args[i + 1] != NULL)
				printf(" ");
		}
	}
	if (!opt)
		printf("\n");
	return (true);
}
