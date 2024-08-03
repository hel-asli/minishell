/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 23:45:06 by hel-asli          #+#    #+#             */
/*   Updated: 2024/08/03 00:04:40 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Include/utils.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	char	*str;
	size_t	i;
	size_t	j;

	str = (char *)big;
	if (!*little)
		return ((char *)big);
	if (len == 0)
		return (NULL);
	i = 0;
	while (str[i] && i < len)
	{
		j = 0;
		while (little[j] == str[i + j] && little[j] && i + j < len)
			j++;
		if (j == ft_strlen(little))
			return (str + i);
		i++;
	}
	return (NULL);
}
