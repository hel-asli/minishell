/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 03:48:06 by hel-asli          #+#    #+#             */
/*   Updated: 2024/08/08 01:50:18 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool quotes_syntax_check(char *line)
{
	bool in_quotes = false;
	char	quote_type = 0;
	int i = 0;

	while (line[i])
	{
		if (!in_quotes && (line[i] == 39 || line[i] == 34))
		{
			in_quotes = true;
			quote_type = line[i];
		}
		else if (in_quotes && line[i] == quote_type)
			in_quotes = false;
		i++;
	}
	return (!in_quotes);
}

t_syntax other_syntax_check(char *line)
{
	t_syntax result;
	int i = -1;
	char **tokens = ft_split(line);
	if (!tokens)
		err_handle("Allocation Fail");
	
	result = SYNTAX_OK;
	while (tokens[++i])
	{
		if (is_redirection(tokens[i]) && !check_redirection(tokens, i))
		{
			result = INVALID_REDIRECTINO;
			break ;
		}
		else if (!ft_strcmp(tokens[i], "<<") && !check_heredoc(tokens, i))
		{
			result = INVALID_HEREDOC;
			break ;
		}
		else if (!ft_strcmp(tokens[i], "|") && !check_pipe(tokens, i))
		{
			result = INVALIDE_PIPE;
			break ;
		}
	}
	return (ft_free(tokens), result);
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
	size_t j = 0;
	size_t i = 0;

	char *new_line = malloc(sizeof(char) * len + 1);
	if (!new_line)
		return (free(line), NULL);
	while (line[i] && j < len)
	{
		if (ft_strstr(&line[i], ">>") || ft_strstr(&line[i], "<<"))
		{
			(new_line[j++] = 32, new_line[j++] = line[i++]);
			(new_line[j++] = line[i++],new_line[j++] = 32);
		}
		else if (ft_strstr(&line[i], ">") || ft_strstr(&line[i], "<") || ft_strstr(&line[i], "|"))
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

void space_to_gar(char *line)
{
    int i;
    bool in_quotes = false;
    char quote_type = 0;
    char nested_quote = 0;


    i = 0;
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
        else if (in_quotes && !nested_quote && 
                 (line[i] == '"' || line[i] == '\'') && 
                 line[i] != quote_type)
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

void pipes_cmds(t_commands **commands, char **pipes)
{
	int i = 0;
	char **tab;
	int arg_count = 0;
	char **arg;
	t_redirect *redirect = NULL;
	int k = 0;
	int j = 0;
	while (pipes[i])
	{
		printf("%d -- > %s\n", i , pipes[i]);
		tab = ft_split(pipes[i]);
		if (!tab)
			puts("ook");
		j = 0;
		printf("j : %d\n", j);
		arg_count = 0;
		printf("arg_count : %d\n", arg_count);
		while (tab[j])
		{
			if (tab[j + 1] && (is_redirection(tab[j]) || !ft_strcmp(tab[j], "<<")))
			{
				ft_lst_add_redir(&redirect, ft_new_redir(tab[j], tab[j + 1]));
				j += 2;
			}
			else
			{
				arg_count++;
				j++;
			}
		}
		printf("--> %d\n", arg_count);
		arg = malloc(sizeof(char *) * (arg_count + 1));
		if (!arg)
			return ; 
		j = 0;
		k = 0;
		printf("--- %d\n", j);
		while (tab[j])
		{
			if (tab[j + 1] && (is_redirection(tab[j]) || !ft_strcmp(tab[j], "<<")))
				j += 2;
			else
			{
				puts("iter");
				arg[k++] = ft_strdup(tab[j++]);
			}
		}
		arg[k] = NULL;
		ft_back_addlst(commands, ft_newlist(arg[0], arg, redirect));
		free(tab);
		i++;
	}
}

void print_cmds(t_commands *cmds)
{
    while (cmds)
    {
        printf("----------------------------------------------\n");
        printf("cmd : %s\n", cmds->cmd);
        
        printf("args : ");
        if (cmds->args)
        {
            int i = 0;
            while (cmds->args[i])
            {
                printf("%s ", cmds->args[i]);
                i++;
            }
        }
        else
        {
            printf("(None) args . ");
        }
        printf("\n");
        
        t_redirect *red = cmds->redirect;    
        printf("redirections : \n");
        if (!red)
        {
            printf("(None) redirection . \n");
        }
        else
        {
            while (red)
            {
                printf("type : %s\n", red->type);
                printf("file : %s\n", red->file);
                red = red->next;
            }
        }
        
        printf("----------------------------------------------\n");
        cmds = cmds->next;    
    }
}

int parse_input(t_parsing *parsing)
{
	char *new_line;
	char **pipes;
	t_syntax syntax;
	new_line = add_spaces(parsing->line);
	parsing->line = new_line;
	// parsing->cmnds->redirect = NULL;
	if (!new_line)
		return (-1);
	if (!quotes_syntax_check(new_line))
		syntax = UNCLOSED_QUOTES;
	else
		syntax = other_syntax_check(new_line);
	
	if (syntax != SYNTAX_OK)
	{
		syntax_err_msg(syntax);
		free(new_line);
		return (1);
	}
	space_to_gar(parsing->line);
	parsing->cmnds = NULL;
	pipes = ft_split_v2(parsing->line, 124);
	for (int i = 0; pipes[i]; i++)
		printf("%s\n", pipes[i]);
	// space_to_gar(parsing->line);
	pipes_cmds(&parsing->cmnds, pipes);
	print_cmds(parsing->cmnds);
	return (0);
}

char *read_input(t_parsing *parsing, const char *prompt)
{
	// char	*line;
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
			continue;
		free(parsing->line);
	}
	return (parsing->line);
}