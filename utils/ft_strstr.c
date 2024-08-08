/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-feng <oel-feng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 23:45:06 by hel-asli          #+#    #+#             */
/*   Updated: 2024/08/08 02:27:46 by oel-feng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	ft_strstr(char *str, char *del)
{
	size_t	i;

	i = 0;
	while (str[i] && del[i] && str[i] == del[i])
		i++;
	return (i == ft_strlen(del));
}
