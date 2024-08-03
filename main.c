/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 00:20:44 by hel-asli          #+#    #+#             */
/*   Updated: 2024/08/03 23:21:36 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Include/minishell.h"
#include "Include/utils.h"
#include "Include/parsing.h"



// void print_splited(char **line)
// {
// 	int i = 0;
// 	while (line[i])
// 	{
// 		printf("-- %s\n", line[i]);
// 		i++;
// 	}
// }

// size_t count_len(char *line)
// {
// 	size_t len = ft_strlen(line);
// 	int i = 0;

// 	while (line[i])
// 	{
// 		if (ft_strstr(&line[i], ">>") || ft_strstr(&line[i], "<<"))
// 		{
// 			len += 2;
// 			i += 2;
// 		}
// 		else if (ft_strstr(&line[i], ">") || ft_strstr(&line[i], "<") || ft_strstr(&line[i], "|"))
// 		{
// 			len += 2;
// 			i++;
// 		}
// 		else
// 			i++;
// 	}

// 	return (len);
// }

// char *add_spaces(char *line)
// {
// 	size_t len = count_len(line);
// 	char *new_line = malloc(sizeof(char) * len + 1);
// 	size_t j = 0;
// 	size_t i = 0;
// 	if (!new_line)
// 		return (NULL);
// 	while (line[i] && j < len)
// 	{
// 		if (ft_strstr(&line[i], ">>") || ft_strstr(&line[i], "<<"))
// 		{
// 			new_line[j++] = 32;
// 			new_line[j++] = line[i++];
// 			new_line[j++] = line[i++];
// 			new_line[j++] = 32;
// 		}
// 		else if (ft_strstr(&line[i], ">") || ft_strstr(&line[i], "<") || ft_strstr(&line[i], "|"))
// 		{
// 			new_line[j++] = 32;
// 			new_line[j++] = line[i++];
// 			new_line[j++] = 32;
// 		}
// 		else
// 		{
// 			new_line[j++] = line[i++];
// 		}
// 	}
// 	new_line[j] = '\0';
// 	free(line);
// 	return (new_line);
// }

// int other_syntax_check(char *line)
// {
// 	int i = 0;
// 	char **sp = ft_split(line);
// 	if (!sp)
// 		return (-1);
	
// 	while (sp[i])
// 	{
// 		if (!ft_strcmp(sp[i], ">>") || !ft_strcmp(sp[i], "<<") || !ft_strcmp(sp[i], ">") || !ft_strcmp(sp[i], "<"))
// 		{
// 			if (i > 0 && (sp[i - 1] == NULL || sp[i + 1] == NULL))
// 					return (1);
// 			else
// 				if (sp[i + 1] == NULL)
// 					return (1);
// 		}
// 		else if (!ft_strcmp(sp[i], "|"))
// 		{
// 			if (i == 0)
// 				return (2);
// 			else if (sp[i+1] == NULL)
// 				return (2);
// 		}
// 		i++;
// 	}
// 	return (3);
// }

int	main(int ac, char **av, char **env)
{
	char	*line;
	((void)ac, (void)av, (void)env);

	// read_input
	line = read_input("minishell: ");
	// if (!quotes_syntax_check(line))
	// 		fprintf(stderr, "syntax error\n");
	// else
	// 		printf("%s\n", line);
	// line = add_spaces(line);
	// int n = other_syntax_check(line);
	// if (n == 1)
	// 	printf("bash: syntax error near unexpected token `newline'\n");
	// else if (n == 2)
	// 	printf("bash: syntax error near unexpected token `|'\n");
	// free(line);
}
