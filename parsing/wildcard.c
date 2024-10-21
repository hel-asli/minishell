/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-feng <oel-feng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 22:40:10 by oel-feng          #+#    #+#             */
/*   Updated: 2024/10/21 22:43:53 by oel-feng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_wildcard(char *str)
{
	int		i;
	bool	db;
	bool	si;

	i = 0;
	db = false;
	si = false;
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

char	**wildcard_expand_helper(char **tab, char **args, int i)
{
	char	**sp;
	char	**new;

	sp = NULL;
	new = NULL;
	args[i] = del_quote(args[i]);
	gar_protect(args[i]);
	sp = wildcard_helper(args[i]);
	if (sp)
		new = re_build_arg(tab, sp);
	fr_args(sp);
	return (new);
}

void	wildcard_redirection(char *file, t_redirect *redirect)
{
	char	**sp;

	sp = wildcard_helper(file);
	if (arr_len(sp) != 1)
	{
		redirect->is_ambgious = true;
		free(redirect->file);
		redirect->file = NULL;
	}
	else
	{
		free(redirect->file);
		redirect->file = ft_strdup(sp[0]);
	}
	free(file);
	fr_args(sp);
}

char	**wildcard_helper(char *arg)
{
	DIR		*dir;
	char	*pwd;
	char	*prefix;
	char	**tab;

	tab = NULL;
	pwd = NULL;
	prefix = ft_strdup("");
	dir = opendir(".");
	if (!dir)
	{
		perror("opendir");
		return (NULL);
	}
	if (arg && arg[0] == '/')
		pwd = getcwd(NULL, 0);
	if (ft_strchr(arg, '/') && (!starts_with(arg, pwd)))
		prefix = str_add_char(ft_strdup(pwd), '/');
	tab = get_files(arg, prefix, dir);
	free(prefix);
	free(pwd);
	closedir(dir);
	return (tab);
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
