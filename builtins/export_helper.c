/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-feng <oel-feng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 21:59:19 by oel-feng          #+#    #+#             */
/*   Updated: 2024/10/21 22:01:17 by oel-feng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	export_print(t_env **exp)
{
	t_env	*export;

	export = *exp;
	while (export)
	{
		if (export->value)
			ft_fprintf(STDOUT_FILENO, "declare -x %s=\"%s\"\n",
				export->key, export->value);
		else
			ft_fprintf(STDOUT_FILENO, "declare -x %s\n", export->key);
		export = export->next;
	}
}

char	*copy_key(char *src)
{
	int		len;
	int		i;
	char	*dest;

	i = 0;
	len = 0;
	while (src[len] && (src[len] != '=' && src[len] != '+'))
		len++;
	dest = (char *)malloc(sizeof(char) * (len + 1));
	while (i < len)
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

void	sort_export(t_env **export)
{
	t_env		*curr;
	t_env		*tmp;
	char		*key;
	char		*value;

	curr = *export;
	while (curr)
	{
		tmp = curr->next;
		while (tmp)
		{
			if (curr->key && ft_strcmp(curr->key, tmp->key) > 0)
			{
				key = curr->key;
				value = curr->value;
				curr->key = tmp->key;
				curr->value = tmp->value;
				tmp->key = key;
				tmp->value = value;
			}
			tmp = tmp->next;
		}
		curr = curr->next;
	}
	export_print(export);
}

bool	env_key_exist(t_env **env, char *key)
{
	t_env	*curr;

	curr = *env;
	while (curr)
	{
		if (!ft_strcmp(curr->key, key))
			return (true);
		curr = curr->next;
	}
	return (false);
}
