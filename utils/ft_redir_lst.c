/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_redir_lst.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 02:29:29 by oel-feng          #+#    #+#             */
/*   Updated: 2024/10/18 00:47:17 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_commands	*ft_newlist(char **args, t_redirect *red)
{
	t_commands	*node;

	node = malloc(sizeof(t_commands));
	if (!node)
		return (NULL);
	node->args = args;
	node->redirect = red;
	node->next = NULL;
	return (node);
}

void	env_clear(t_env **env)
{
	t_env	*cur;
	t_env	*tmp;

	if (!env)
		return ;
	cur = *env;
	while (cur)
	{
		tmp = cur;
		cur = cur->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
		tmp = NULL;
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

void	clear_redirect(t_redirect **redirect)
{
	t_redirect *curr;
	t_redirect *tmp;

	if (!redirect)
		return ;
	puts("ok");
	curr = *redirect;
	tmp = NULL;
	while (curr)
	{
		tmp = curr;
		curr = curr->next;
		free(tmp->file);
		if (tmp->heredoc_fd != -1)
			close(tmp->heredoc_fd);
		free(tmp);
		tmp = NULL;
	}
	*redirect = NULL;
}

t_redirect	*ft_new_redir(char *type, char *file)
{
	t_redirect	*node;

	node = malloc(sizeof(t_redirect));
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
	node->file = ft_strdup(file);
	node->heredoc_fd = -1;
	node->is_ambgious = false;
	node->next = NULL;
	return (node);
}
