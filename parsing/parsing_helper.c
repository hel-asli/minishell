/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 02:46:47 by oel-feng          #+#    #+#             */
/*   Updated: 2024/10/22 10:43:09 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	check_pattern(const char *pattern, const char *str)
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
				return (true);
			while (*str)
			{
				if (check_pattern(pattern, str))
					return (true);
				str++;
			}
			return (false);
		}
		else if (*pattern == *str)
			(1) && (str++, pattern++);
		else
			return (false);
	}
	while (*pattern == '*')
		pattern++;
	return (*str == '\0' && *pattern == '\0');
}

char	**get_files(char *str, char *prefix, DIR *dir)
{
	char			**tab;
	struct dirent	*entity;
	char			*new_str;
	char			**sp;

	tab = NULL;
	entity = readdir(dir);
	new_str = NULL;
	if (ft_strchr(str, '/') && is_not_sub(str, prefix))
		tab = add_arr(tab, str);
	while (entity)
	{
		if (str[ft_strlen(str) - 1] == '/')
		{
			sp = ft_split_v2(str, '/');
			if (entity->d_type == DT_DIR && check_pattern(sp[arr_len(sp) - 1],
					entity->d_name))
			{
				new_str = str_add_char(ft_strdup(entity->d_name), '/');
				tab = add_arr(tab, new_str);
				free(new_str);
			}
			fr_args(sp);
		}
		else
		{
			sp = ft_split_v2(str, '/');
			if (check_pattern(sp[arr_len(sp) - 1], entity->d_name))
			{
				new_str = ft_strjoin(ft_strdup(prefix),
						ft_strdup(entity->d_name));
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

char	**wildcard_expand(char **args, int i)
{
	DIR		*dir;
	char	*pwd;
	char	**sp;
	char	*prefix;
	char	**tab;

	tab = NULL;
	sp = NULL;
	prefix = ft_strdup("");
	pwd = NULL;
	dir = opendir(".");
	if (!dir)
	{
		perror("opendir");
		return (NULL);
	}
	while (args[i])
	{
		if (ft_strchr(args[i], '*') && check_wildcard(args[i]))
		{
			if (args[i] && args[i][0] == '/')
				pwd = getcwd(NULL, 0);
			if (ft_strchr(args[i], '/') && (!starts_with(args[i], pwd)))
				prefix = str_add_char(ft_strdup(pwd), '/');
			free(pwd);
			sp = get_files(args[i], prefix, dir);
			tab = re_build_arg(tab, sp);
			fr_args(sp);
		}
		else
			tab = add_arr(tab, args[i]);
		i++;
	}
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
				tab = wildcard_expand(tab, 0);
		}
		else if (ft_strchr(args[i], '*') && check_wildcard(args[i]))
			tab = wildcard_expand_helper(tab, args, i);
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

void	expand_redirect(t_redirect *redirect, t_shell *shell)
{
	t_redirect	*tmp;
	char		*file;

	(1) && (tmp = redirect, file = NULL);
	while (tmp)
	{
		if (tmp->type != HEREDOC_INPUT)
		{
			file = expand_arg(tmp->file, shell);
			if (!file)
				file = ft_strdup("");
			if (file && ft_strchr(file, '*') && check_wildcard(tmp->file))
				wildcard_redirection(file, tmp);
			else if (!file && !ft_strchr(tmp->file, '"')
				&& !ft_strchr(tmp->file, '\''))
				(1) && (tmp->is_ambgious = true, free(tmp->file), tmp->file = file);
			else if (file && check_var(tmp->file) && ft_strchr(file, 32))
				(1) && (tmp->is_ambgious = true, free(tmp->file), tmp->file = file);
			else
				(1) && (free(tmp->file), tmp->file = del_quote(file));
		}
		else
			tmp->file = del_quote(tmp->file); // cat << $'d' 
		tmp = tmp->next;
	}
}
