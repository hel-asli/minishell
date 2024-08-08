/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_v2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-feng <oel-feng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 03:10:30 by oel-feng          #+#    #+#             */
/*   Updated: 2024/08/08 02:37:47 by oel-feng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
