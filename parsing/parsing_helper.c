/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-feng <oel-feng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 02:46:47 by oel-feng          #+#    #+#             */
/*   Updated: 2024/10/24 01:28:31 by oel-feng         ###   ########.fr       */
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

char	**get_files(char *str, t_wildcard *w)
{
	char			**tab;
	char			*new_str;

	(1) && (tab = NULL, new_str = NULL);
	if (ft_strchr(str, '/') && is_not_sub(str, w->prefix))
		tab = add_arr(tab, str);
	while (w->entity)
	{
		if (str[ft_strlen(str) - 1] == '/')
		{
			new_str = wildcard_dir(w->entity, str);
			tab = add_arr(tab, new_str);
			free(new_str);
		}
		else
		{
			new_str = wildcard_file(w->entity, str, w->prefix);
			tab = add_arr(tab, new_str);
			free(new_str);
		}
		w->entity = readdir(w->dir);
	}
	if (!tab)
		tab = add_arr(tab, str);
	return (tab);
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
				tab = wildcard_expand(tab, -1);
		}
		else if (ft_strchr(args[i], '*') && check_wildcard(args[i]))
			tab = wildcard_expand_helper(tab, args, i - 1);
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
				ambigious_check(tmp, file);
			else if (file && check_var(tmp->file) && ft_strchr(file, 32))
				ambigious_check(tmp, file);
			else
				(1) && (free(tmp->file), tmp->file = del_quote(file));
		}
		else
			tmp->file = del_quote(tmp->file);
		tmp = tmp->next;
	}
}
