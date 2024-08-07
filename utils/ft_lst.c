/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lst.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-feng <oel-feng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 03:10:56 by hel-asli          #+#    #+#             */
/*   Updated: 2024/08/07 03:29:40 by oel-feng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


t_env	*ft_lstlast(t_env *env)
{
	if (!env)
		return (NULL);
	while (env->next != NULL)
		env = env->next;
	return (env);
}

void	ft_lstadd_back(t_env **lst, t_env *new)
{
	t_env *last;

	if (lst)
	{
		if (*lst == NULL)
		{
			*lst = new;
		}
		else
		{
			last = ft_lstlast(*lst);
			last->next = new;
		}
	}
}

t_env	*ft_lstnew(char *key, char *value)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = key;
	node->value =value;
	node->next = NULL;
	return (node);
}

void	ft_back_addlst(t_commands **lst, t_commands *new)
{
	t_commands *last;

	if (lst)
	{
		if (*lst == NULL)
		{
			*lst = new;
		}
		else
		{
			last = ft_last(*lst);
			last->next = new;
		}
	}
}

t_commands	*ft_last(t_commands *node)
{
	if (!node)
		return (NULL);
	while (node->next != NULL)
		node = node->next;
	return (node);
}

t_commands *ft_newlist(char *cmd, char **args)
{
	t_commands *node;

	node = malloc(sizeof(t_commands));
	if (!node)
		return (NULL);
	node->cmd = cmd;
	node->args = args;
	node->next = NULL;
	return (node);
}

void env_clear(t_env **env)
{
	t_env *cur = *env;
	t_env *tmp;

	if (!env)	
		return ;
	while (cur)
	{
		tmp = cur;
		cur = cur->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
		// cur = tmp;
	}
	*env = NULL;
}
