/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 02:46:47 by oel-feng          #+#    #+#             */
/*   Updated: 2024/10/20 21:54:43 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


char *check_value(char *value)
{
	if (!value[0])
		return (free(value), NULL);
	return (value);
}

char *expand_arg(char *arg, t_env *env, t_shell *shell)
{
	char	*value;
	bool	quotes[2];
	int		i;

	(1) && (i = 0, quotes[0] = false, quotes[1] = false);
	value = ft_strdup("");
	if (!value)
		err_handle("allocation");
	while (arg[i])
	{
		if (arg[i] == '\'' && !quotes[1])
			(1) && (quotes[0] = !quotes[0], i++);
		else if (arg[i] == '"' && !quotes[0])
			(1) && (quotes[1] = !quotes[1], i++);
		else if (arg[i] == '$' && !quotes[0])
		{
			i++;
			if ((arg[i] == '"' || arg[i] == '\'') && !quotes[1] && !quotes[0])
				continue ;
			value = ft_strjoin(value, get_new_value(shell, arg, &i));
		}
		else
			(1) && (value = str_add_char(value, arg[i]), i++);
	}
	return (check_value(value));
}

int starts_with(char *start, char *str)
{
    int i = 0;

    while (str[i] && start[i])
    {
        if (str[i] == start[i])
            i++;
        else
            break;
    }
    if (!str[i])
        return (0);
    return (1);
}

int check_wildcard(char *str)
{
    int i;
    bool db = false;
    bool si = false;

    i = 0;
    while (str[i])
    {
        if (str[i] == '\'' && !db)
            si = !si;
        else if (str[i] == '"' && !si)
            db = !db;
        else if ((db || si) && str[i] == '*')
            return (false);
        i++;
    }
    return (true);
}

bool check_pattern(const char *pattern, const char *str)
{
	if (*pattern == '*' && *str == '.')
		return (false);
    while (*str && *pattern)
    {
        if (*pattern == '*')
        {
            while (*pattern == '*')
                pattern++;
            if (*pattern == '\0')
                return true;
            while (*str)
            {
                if (check_pattern(pattern, str))
                    return true;
                str++;
            }
            return false;
        }
        else if (*pattern == *str)
        {
            str++;
            pattern++;
        }
        else
            return false;
    }
    while (*pattern == '*')
        pattern++;
    return (*str == '\0' && *pattern == '\0');
}

char **get_files(char *str, char *prefix, DIR *dir)
{
    char **tab;
    struct dirent *entity;
    DIR *sub;

    tab = NULL;
    entity = readdir(dir);
    char *new_str;
    char **sp;
    new_str = NULL;

    while (entity)
    {
            if (str[ft_strlen(str) - 1] == '/')
            {
                sp = ft_split_v2(str, '/');
                if (entity->d_type == DT_DIR && check_pattern(sp[arr_len(sp) - 1],entity->d_name))
                    tab = add_arr(tab, str_add_char(ft_strdup(entity->d_name), '/'));
                fr_args(sp);
            }
            else
            {
                sp = ft_split_v2(str, '/');
                if (check_pattern(sp[arr_len(sp) - 1], entity->d_name))
				{

					new_str = ft_strjoin(ft_strdup(prefix), ft_strdup(entity->d_name));
                    tab = add_arr(tab, new_str);
					free(new_str);
				}
                fr_args(sp);
            }
        entity = readdir(dir);
    }
	if (!tab)
		tab = add_arr(tab, str);
    return (tab);
}

bool is_not_sub(const char *str, const char *pwd)
{
    int i = 0;
    int j = 0;

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

char **wildcard_helper(char *arg)
{
		DIR *dir;
        char *pwd;
        char *prefix;
		char **tab;

        pwd = getcwd(NULL, 0);
		tab = NULL;
        if (!pwd)
        {
            perror("getcwd");
            return (NULL);
        }
        prefix = ft_strdup("");
        dir = opendir(".");
        if (!dir)
        {
            perror("opendir");
            return (NULL);
        }
		if (ft_strchr(arg, '/') && is_not_sub(arg, pwd))
			tab = add_arr(tab, arg);
		else
		{
			if (ft_strchr(arg, '/') && (!starts_with(arg, pwd)))
                prefix = str_add_char(ft_strdup(pwd), '/');
			tab = get_files(arg, prefix, dir);
		}
		free(pwd);
		closedir(dir);
		return (tab);
}

char **wildcard_expand(char **args)
{
		DIR *dir;
        char *pwd;
		char **sp;
        char *prefix;
		char **tab;
		int i = 0;

        pwd = getcwd(NULL, 0);
		tab = NULL;
		sp = NULL;
        if (!pwd)
        {
            perror("getcwd");
            return (NULL);
        }
        prefix = ft_strdup("");
        dir = opendir(".");
        if (!dir)
        {
            perror("opendir");
            return (NULL);
        }
		while (args[i])
		{
			if (ft_strchr(args[i], '/') && is_not_sub(args[i], pwd))
			{
				tab = add_arr(tab, args[i]);
			}
			else if (ft_strchr(args[i], '*') && check_wildcard(args[i]))
			{
				if (ft_strchr(args[i], '/') && (!starts_with(args[i], pwd)))
						prefix = str_add_char(ft_strdup(pwd), '/');
				sp = get_files(args[i], prefix, dir);
				tab = re_build_arg(tab,sp);
				fr_args(sp);
			}
			else
				tab = add_arr(tab, args[i]);
			i++;
		}
		free(pwd);
		closedir(dir);
		return (fr_args(args), free(prefix), tab);
}

char	**expand_args(char **args, t_shell *shell)
{
	int		i;
	char	*new_arg;
	char	**sp;
	char	**tab;

	(1) && (i = 0, new_arg = NULL, sp = NULL, tab = NULL);
	while (args[i])
	{
		if (ft_strchr(args[i], '$'))
		{
			tab = replace_tab(tab, args[i], shell);
			if (tab && check_var(args[i]))
				tab = wildcard_expand(tab);
			// segfault on "$a" => a = echo *.c
		}
		else if (ft_strchr(args[i], '*') && check_wildcard(args[i]))
		{
			args[i] = del_quote(args[i]);
			gar_protect(args[i]);
			sp = wildcard_helper(args[i]);
			if (sp)
				tab = re_build_arg(tab, sp);
			fr_args(sp);
		}
		else
		{
			args[i] = del_quote(args[i]);
			gar_protect(args[i]);
			tab = add_arr(tab, args[i]);
		}
		i++;
	}
	return (fr_args(args), tab);
}

void	expand_redirect(t_redirect *redirect, t_env *env, t_shell *shell)
{
	t_redirect	*tmp;
	char		*file;
	char		**sp;

	(1) && (tmp = redirect, file = NULL);
	while (tmp)
	{
		if (tmp->type != HEREDOC_INPUT)
		{
			file = expand_arg(tmp->file, env, shell);
			fprintf(stderr, "--%s\n", file);
			if (file && ft_strchr(file, '*') && check_wildcard(tmp->file))
			{
				sp = wildcard_helper(file);
				if (arr_len(sp) != 1)
				{
					tmp->is_ambgious = true;
					tmp->file = NULL;
				}
				else
				{
					free(tmp->file);
					tmp->file = ft_strdup(sp[0]);
				}
				fr_args(sp);
			}
			else if (!file && !ft_strchr(tmp->file, '"')
				&& !ft_strchr(tmp->file, '\''))
			{
				puts("heree");
				tmp->is_ambgious = true;
				free(tmp->file);
				tmp->file = file;
			}
			else if (file && check_var(tmp->file) && ft_strchr(file, 32))
			{
				puts("here");
				tmp->is_ambgious = true;
				free(tmp->file);
				tmp->file = file;
			}
			else
				tmp->file = del_quote(tmp->file);
		}
		else
			tmp->file = del_quote(tmp->file);
		tmp = tmp->next;
	}
}

void	process_pipe_cmds(t_shell **shell, char **pipes)
{
	int			i;
	char		**tab;
	char		**args;
	t_redirect	*redirect;

	(1) && (i = 0, redirect = NULL);
	while (pipes[i])
	{
		tab = ft_split(pipes[i]);
		if (!tab)
			err_handle("Allocation Faile");
		(1) && (protect_tab(tab), redirect = build_redirection(tab));
		args = args_allocation(tab, count_non_redirection_arg_size(tab));
		(1) && (fr_args(tab), args = expand_args(args, *shell));
		expand_redirect(redirect, (*shell)->env, *shell);
		if (!args && !redirect)
		{
			i++;
			continue ;
		}
		ft_back_addlst(&(*shell)->commands, ft_newlist(args, redirect));
		i++;
	}
	fr_args(pipes);
}

void	print_cmds(t_commands *cmds)
{
	int			i;
	t_redirect	*red;

	while (cmds)
	{
		printf("----------------------------------------------\n");
		printf("args : ");
		if (cmds->args)
		{
			i = 0;
			while (cmds->args[i])
			{
				printf("{%s} ", cmds->args[i]);
				i++;
			}
		}
		else
			printf("(None) args . ");
		printf("\n");
		red = cmds->redirect;
		printf("redirections : \n");
		if (!red)
			printf("(None)\n");
		else
		{
			while (red)
			{
				printf("type : %u\n", red->type);
				printf("expanded : %d\n", red->expanded);
				printf("is_ambigous : %d\n", red->is_ambgious);
				printf("file : {%s}\n", red->file);
				red = red->next;
			}
		}
		printf("----------------------------------------------\n");
		cmds = cmds->next;
	}
}