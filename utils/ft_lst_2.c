/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lst_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-feng <oel-feng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 19:46:54 by oel-feng          #+#    #+#             */
/*   Updated: 2024/10/18 13:24:46 by oel-feng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_lstsize(t_commands *lst)
{
	int			len;
	t_commands	*list;

	len = 0;
	list = lst;
	while (list)
	{
		len++;
		list = list->next;
	}
	return (len);
}

t_commands	*ft_last(t_commands *node)
{
	if (!node)
		return (NULL);
	while (node->next != NULL)
		node = node->next;
	return (node);
}
