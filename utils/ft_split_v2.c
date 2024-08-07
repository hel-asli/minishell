/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_v2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-feng <oel-feng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 03:10:30 by oel-feng          #+#    #+#             */
/*   Updated: 2024/08/07 03:34:38 by oel-feng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static size_t	ft_strlcpy(char *dest, const char *src, size_t size)
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

static char	*ft_substr(char const *s, unsigned int start, size_t len)
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

static int	ft_words(const char *s, char c)
{
	int	a;
	int	words;
	int	counter;

	a = 0;
	words = 0;
	while (s[a])
	{
		counter = 0;
		while (s[a] && s[a] == c)
			a++;
		while (s[a] && s[a] != c)
		{
			counter = 1;
			a++;
		}
		if (counter == 1)
			words++;
	}
	return (words);
}

static void	ft_memoryfree(char **res, int a)
{
	int	b;

	b = 0;
	while (b < a)
	{
		free(res[b]);
		b++;
	}
	free(res);
}

static char	**ft_fill(const char *s, char **res, char c)
{
	int			a;
	char		*str;

	a = 0;
	while (*s)
	{
		str = (char *)s;
		if (*s != c)
		{
			while (*s != c && *s)
				s++;
			res[a] = ft_substr(str, 0, s - str);
			if (!res[a])
			{
				ft_memoryfree(res, a);
				return (0);
			}
			a++;
		}
		else
			s++;
	}
	res[a] = 0;
	return (res);
}

char	**ft_split_v2(const char *s, char c)
{
	char	**res;
	int		words;

	if (!s)
		return (0);
	words = ft_words(s, c);
	res = malloc(sizeof(char *) * (words + 1));
	if (!res)
		return (0);
	res = ft_fill(s, res, c);
	return (res);
}