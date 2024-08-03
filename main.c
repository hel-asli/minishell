/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 00:20:44 by hel-asli          #+#    #+#             */
/*   Updated: 2024/08/03 03:32:11 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Include/minishell.h"
#include "Include/utils.h"

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

void err_handle(char *str)
{
	ft_putendl_fd(str, STDERR_FILENO);
	exit(EXIT_FAILURE);
}

bool is_space(char c)
{
	return (c == 32 || c == 9);
}

bool empty_str(char *line)
{
	int i;

	i = 0;
	while (line[i])
	{
		if (!is_space(line[i]))
			return (false);
		i++;
	}

	return (true);
}

void print_splited(char **line)
{
	int i = 0;
	while (line[i])
	{
		printf("-- %s\n", line[i]);
		i++;
	}
}
bool ft_strstr(char *str, char *del)
{
	size_t i = 0;

	while (str[i] && del[i] && str[i] == del[i])	
		i++;
	return (i == ft_strlen(del));
}

size_t count_len(char *line)
{
	size_t len = ft_strlen(line);
	int i = 0;

	while (line[i])
	{
		if (ft_strstr(&line[i], ">>") || ft_strstr(&line[i], "<<"))
		{
			len += 2;
			i += 2;
		}
		else if (ft_strstr(&line[i], ">") || ft_strstr(&line[i], "<") || ft_strstr(&line[i], "|"))
		{
			len += 2;
			i++;
		}
		else
			i++;
	}

	return (len);
}

char *add_spaces(char *line)
{
	size_t len = count_len(line);
	char *new_line = malloc(sizeof(char) * len + 1);
	size_t j = 0;
	size_t i = 0;
	if (!new_line)
		return (NULL);
	while (line[i] && j < len)
	{
		if (ft_strstr(&line[i], ">>") || ft_strstr(&line[i], "<<"))
		{
			new_line[j++] = 32;
			new_line[j++] = line[i++];
			new_line[j++] = line[i++];
			new_line[j++] = 32;
		}
		else if (ft_strstr(&line[i], ">") || ft_strstr(&line[i], "<") || ft_strstr(&line[i], "|"))
		{
			new_line[j++] = 32;
			new_line[j++] = line[i++];
			new_line[j++] = 32;
		}
		else
		{
			new_line[j++] = line[i++];
		}
	}
	new_line[j] = '\0';
	free(line);
	return (new_line);
}


int other_syntax_check(char *line)
{
	int i = 0;
	char **sp = ft_split(line);
	if (!sp)
		return (-1);
	
	while (sp[i])
	{
		if (!ft_strcmp(sp[i], ">>") || !ft_strcmp(sp[i], "<<") || !ft_strcmp(sp[i], ">") || !ft_strcmp(sp[i], "<"))
		{
			if (i > 0 && (sp[i - 1] == NULL || sp[i + 1] == NULL))
					return (ft_free(sp), 1);
			else
				if (sp[i + 1] == NULL)
					return (ft_free(sp), 1);
		}
		else if (!ft_strcmp(sp[i], "|"))
		{
			if (i == 0)
				return (ft_free(sp), 2);
			else if (sp[i+1] == NULL)
				return (ft_free(sp), 2);
		}
		i++;
	}
	return (3);
}

int	main(int ac, char **av, char **env)
{
	char	*line;
	((void)ac, (void)av, (void)env);

	while (true)
	{
		line = readline("minishell: ");
		if (!line)
		{
			err_handle("exit");
		}
		add_history(line);
		if (!ft_strlen(line) || empty_str(line))
		{
			free(line);
			continue ;
		}
		else if (!quotes_syntax_check(line))
			fprintf(stderr, "syntax error\n");
		else
			printf("%s\n", line);
		line = add_spaces(line);
		int n = other_syntax_check(line);
		if (n == 1)
			printf("bash: syntax error near unexpected token `newline'\n");
		else if (n == 2)
			printf("bash: syntax error near unexpected token `|'\n");

		free(line);
	}
	return (0);
}
