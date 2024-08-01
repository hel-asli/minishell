/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 00:20:44 by hel-asli          #+#    #+#             */
/*   Updated: 2024/08/01 02:56:00 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool quotes_syntax_check(char *line)
{
	bool check = false;
	char	c = 0;
	int i = 0;

	while (line[i])
	{
		if (!check && (line[i] == 39 || line[i] == 34))
		{
			check = true;
			c = line[i];
		}
		else if (check && line[i] == c)
			check = false;
		i++;
	}
	return (!check);
}

int main(int ac, char **av, char **env)
{
	char	*line;
	(void)ac;
	(void)av;
	(void)env;

	while (true)
	{
		line = readline("minishell: ");
		add_history(line);
		if (!quotes_syntax_check(line))
			fprintf(stderr, "syntax error\n");
		else
			printf("%s\n", line);
		free(line);
	}
}