/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 00:21:14 by hel-asli          #+#    #+#             */
/*   Updated: 2024/08/01 23:37:58 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	count_words(char const *s)
{
	int		count;

	if (!s)
		return (0);
	count = 0;
	while (*s)
	{
		if ((*s != 9 && *s != 32))
		{
			count++;
			while ((*s != 32 && *s != 9) && *s)
				s++;
		}
		else
			s++;
	}
	return (count);
}

char	**ft_free(char **split)
{
	int	i;

	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
	return (NULL);
}

char	*split_word(char *str)
{
	int		i;
	int		j;
	char	*word;

	i = 0;
	j = 0;
	while (str[i] && (str[i] != 32 && str[i] != 9))
		i++;
	word = (char *)malloc(sizeof(char) * (i + 1));
	if (!word)
		return (NULL);
	while (j < i)
	{
		word[j] = str[j];
		j++;
	}
	word[j] = '\0';
	return (word);
}

char	**ft_split(char const *s)
{
	int		i;
	char	**split;

	i = 0;
	split = (char **)malloc(sizeof(char *) * (count_words(s) + 1));
	if (!split || !s)
		return (NULL);
	while (*s)
	{
		while (*s && (*s == 9 || *s == 32))
			s++;
		if (*s)
		{
			split[i] = split_word((char *)s);
			if (!split[i])
				return (ft_free(split));
			i++;
			s++;
		}
		while (*s && (*s != 9 && *s != 32))
			s++;
	}
	split[i] = NULL;
	return (split);
}