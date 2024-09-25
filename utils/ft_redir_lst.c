/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_redir_lst.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 02:29:29 by oel-feng          #+#    #+#             */
/*   Updated: 2024/09/24 22:56:11 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_commands	*ft_newlist(char *cmd, char **args, t_redirect *red)
{
	t_commands	*node;

	node = malloc(sizeof(t_commands));
	if (!node)
		return (NULL);
	if (!cmd)
		node->cmd = cmd;
	else
		node->cmd = ft_strdup(cmd);
	node->args = args;
	node->redirect = red;
	node->next = NULL;
	return (node);
}

void	env_clear(t_env **env)
{
	t_env	*cur;
	t_env	*tmp;

	cur = *env;
	if (!env)
		return ;
	while (cur)
	{
		tmp = cur;
		cur = cur->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
	}
	*env = NULL;
}

void	ft_lst_add_redir(t_redirect **lst, t_redirect *new)
{
	t_redirect	*last;

	if (lst)
	{
		if (*lst == NULL)
			*lst = new;
		else
		{
			last = ft_last_redir(*lst);
			last->next = new;
		}
	}
}

t_redirect	*ft_last_redir(t_redirect *node)
{
	if (!node)
		return (NULL);
	while (node->next != NULL)
		node = node->next;
	return (node);
}

t_redirect	*ft_new_redir(char *type, char *file)
{
	t_redirect	*node;

	node = malloc(sizeof(t_commands));
	node->expanded = false;
	if (!node)
		return (NULL);
	if (!ft_strcmp("<<", type))	
	{
		node->type = HEREDOC_INPUT;
		if (!in_quotes(file))
			node->expanded = true;
	}
	else if (!ft_strcmp("<", type))
		node->type = INPUT;
	else if (!ft_strcmp(">", type))
		node->type = OUT_TRUNC;
	else if (!ft_strcmp(">>", type))
		node->type = OUT_APPEND;
	node->file = file;
	node->is_ambgious = false;
	node->next = NULL;
	return (node);
}

t_redirect	*ft_new_redir_v2(t_red type, char *file, bool expanded)
{
	t_redirect	*node;

	node = malloc(sizeof(t_commands));
	if (!node)
		return (NULL);
	node->type = type;
	node->file = file;
	node->expanded = expanded;
	node->next = NULL;
	return (node);
}