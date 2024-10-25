/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 22:40:10 by oel-feng          #+#    #+#             */
/*   Updated: 2024/10/25 05:15:16 by hel-asli         ###   ########.fr       */
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
	if (sp && !sp[0])
		sp = add_arr(sp, args[i]);
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
	t_wildcard	wildcard;
	char		**tab;

	tab = NULL;
	wild_init(&wildcard);
	if (arg && arg[0] == '/')
		wildcard.pwd = getcwd(NULL, 0);
	if (arg && arg[0] == '.' && arg[1] && arg[1] == '/')
	{
		free(wildcard.prefix);
		wildcard.prefix = ft_strdup("./");
	}
	if (ft_strchr(arg, '/') && (!starts_with(arg, wildcard.pwd)))
	{
		free(wildcard.prefix);
		wildcard.prefix = str_add_char(ft_strdup(wildcard.pwd), '/');
	}
	tab = get_files(arg, &wildcard);
	wild_destory(&wildcard);
	return (tab);
}

char	*wildcard_file(struct dirent *entity, char *str, char *prefix)
{
	char	**sp;
	char	*new_str;

	sp = ft_split_v2(str, '/');
	new_str = NULL;
	if (check_pattern(sp[arr_len(sp) - 1], entity->d_name))
	{
		new_str = ft_strjoin(ft_strdup(prefix),
				ft_strdup(entity->d_name));
	}
	fr_args(sp);
	return (new_str);
}
