/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 03:48:06 by hel-asli          #+#    #+#             */
/*   Updated: 2024/08/09 03:52:16 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

size_t	count_len(char *line)
{
	int		i;
	size_t	len;

	i = 0;
	len = ft_strlen(line);
	while (line[i])
	{
		if (ft_strstr(&line[i], ">>") || ft_strstr(&line[i], "<<"))
		{
			len += 2;
			i += 2;
		}
		else if (ft_strstr(&line[i], ">")
			|| ft_strstr(&line[i], "<") || ft_strstr(&line[i], "|"))
		{
			len += 2;
			i++;
		}
		else
			i++;
	}
	return (len);
}

char	*add_spaces(char *line)
{
	size_t	j;
	size_t	i;
	size_t	len;
	char	*new_line ;

	i = 0;
	j = 0;
	len = count_len(line);
	new_line = malloc(sizeof(char) * len + 1);
	if (!new_line)
		return (free(line), NULL);
	while (line[i] && j < len)
	{
		if (ft_strstr(&line[i], ">>") || ft_strstr(&line[i], "<<"))
		{
			(new_line[j++] = 32, new_line[j++] = line[i++]);
			(new_line[j++] = line[i++],new_line[j++] = 32);
		}
		else if (ft_strstr(&line[i], ">")
			|| ft_strstr(&line[i], "<") || ft_strstr(&line[i], "|"))
		{
			new_line[j++] = 32;
			new_line[j++] = line[i++];
			new_line[j++] = 32;
		}
		else
			new_line[j++] = line[i++];
	}
	new_line[j] = '\0';
	return (free(line), new_line);
}

// void space_to_gar(char *line)
// {
//     int i = 0;
//     bool in_quotes = false;
//     char quote_type = 0;

//     while (line[i])
//     {
//         if (line[i] == '"' || line[i] == '\'')
//         {
//             if (!in_quotes)
//             {
//                 in_quotes = true;
//                 quote_type = line[i];
//             }
//             else if (line[i] == quote_type)
//             {
//                 in_quotes = false;
//             }
//             else
//             {
//                 line[i] *= -1;
//             }
//         }
//         else if (in_quotes)
//         {
//             line[i] *= -1;
//         }
//         i++;
//     }
// }


void	space_to_gar(char *line)
{
	int		i;
	bool	in_quotes;
	char	quote_type;
	char	nested_quote;

	i = 0;
	quote_type = 0;
	nested_quote = 0;
	in_quotes = false;
	while (line[i])
	{
		if (!in_quotes && (line[i] == '"' || line[i] == '\''))
		{
			in_quotes = true;
			quote_type = line[i];
		}
		else if (in_quotes && line[i] == quote_type)
		{
			if (!nested_quote)
				in_quotes = false;
			else
				nested_quote = 0;
		}
		else if (in_quotes && !nested_quote
			&& (line[i] == '"' || line[i] == '\'')
			&& line[i] != quote_type)
		{
			nested_quote = line[i];
			line[i] *= -1;
		}
		else if (in_quotes)
		{
			line[i] *= -1;
		}
		i++;
	}
}

int	parse_input(t_parsing *parsing)
{
	char		*new_line;
	char		**pipes;
	t_syntax	syntax;

	new_line = add_spaces(parsing->line);
	parsing->line = new_line;
	if (!new_line)
		return (-1);
	if (!quotes_syntax_check(new_line))
		syntax = UNCLOSED_QUOTES;
	else
		syntax = other_syntax_check(new_line);
	if (syntax != SYNTAX_OK)
		return (syntax_err_msg(syntax), free(new_line), 1);
	space_to_gar(parsing->line);
	parsing->cmnds = NULL;
	pipes = ft_split_v2(parsing->line, 124);
	parsing->cmnds = NULL;
	pipes_cmds(&parsing->cmnds, pipes);
	print_cmds(parsing->cmnds);
	return (0);
}

char	*read_input(t_parsing *parsing, const char *prompt)
{
	while (true)
	{
		parsing->line = readline(prompt);
		add_history(parsing->line);
		if (!parsing->line)
			err_handle("exit");
		if (!ft_strlen(parsing->line) || empty_str(parsing->line))
		{
			free(parsing->line);
			continue ;
		}
		if (parse_input(parsing) == 1)
			continue ;
		free(parsing->line);
	}
	return (parsing->line);
}
