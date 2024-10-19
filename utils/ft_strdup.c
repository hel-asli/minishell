/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-feng <oel-feng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 04:45:33 by hel-asli          #+#    #+#             */
/*   Updated: 2024/10/18 13:27:00 by oel-feng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_strndup(const char *str, int index)
{
	int		i;
	char	*ptr;	

	i = 0;
	ptr = malloc(sizeof(char) * (index + 1));
	if (!str)
		return (NULL);
	while (str[i] && i < index)
	{
		ptr[i] = str[i];
		i++;
	}
	ptr[i] = 0;
	return (ptr);
}
