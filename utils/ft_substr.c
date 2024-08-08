/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-feng <oel-feng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 02:36:03 by oel-feng          #+#    #+#             */
/*   Updated: 2024/08/08 02:36:42 by oel-feng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

size_t	ft_strlcpy(char *dest, const char *src, size_t size)
{
	size_t	i;

	i = 0;
	if (size != 0)
	{
		while (i < size - 1 && src[i] != '\0')
		{
			dest[i] = src[i];
			i++;
		}
		dest[i] = '\0';
	}
	return (ft_strlen(src));
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*res;
	size_t	sl;
	size_t	a;

	if (!s)
		return (0);
	sl = ft_strlen(s);
	a = 0;
	if (start < sl)
		a = sl - start;
	if (a > len)
		a = len;
	res = (char *)malloc(sizeof(char) * (a + 1));
	if (!res)
		return (0);
	if (a == 0)
		res[a] = '\0';
	else
		ft_strlcpy(res, s + start, a + 1);
	return (res);
}
