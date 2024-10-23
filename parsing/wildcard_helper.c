/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_helper.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-feng <oel-feng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 04:42:32 by oel-feng          #+#    #+#             */
/*   Updated: 2024/10/23 04:43:23 by oel-feng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	wild_init(t_wildcard *wildcard)
{
	wildcard->prefix = ft_strdup("");
	if (!wildcard)
		err_handle("Maloc");
	wildcard->dir = opendir(".");
	if (!wildcard->dir)
	{
		perror("opendir");
	}
	wildcard->entity = readdir(wildcard->dir);
	wildcard->pwd = NULL;
}

void	wild_destory(t_wildcard *wildcard, int flag)
{
	closedir(wildcard->dir);
	if (flag)
		free(wildcard->pwd);
	free(wildcard->prefix);
}

bool	wild_conditon(char *arg, char *pwd)
{
	return (ft_strchr(arg, '/') && (!starts_with(arg, pwd)));
}

char	**wildcard_expand(char **args, int i)
{
	t_wildcard	wildcard;
	char		**sp;
	char		**tab;

	tab = NULL;
	sp = NULL;
	wild_init(&wildcard);
	while (args[i])
	{
		if (ft_strchr(args[i], '*') && check_wildcard(args[i]))
		{
			if (args[i] && args[i][0] == '/')
				wildcard.pwd = getcwd(NULL, 0);
			if (wild_conditon(args[i], wildcard.pwd))
				wildcard.prefix = str_add_char(ft_strdup(wildcard.pwd), '/');
			free(wildcard.pwd);
			sp = get_files(args[i], &wildcard);
			tab = re_build_arg(tab, sp);
			fr_args(sp);
		}
		else
			tab = add_arr(tab, args[i]);
		i++;
	}
	return (wild_destory(&wildcard, 0), fr_args(args), tab);
}
