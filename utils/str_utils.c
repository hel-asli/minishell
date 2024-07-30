/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 00:04:29 by hel-asli          #+#    #+#             */
/*   Updated: 2024/07/31 00:24:38 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

size_t	ft_strlen(const char *str)
{
	size_t	len;

	len = 0;
	while (*str++ != '\0')
		len++;
	return (len);
}

char	*ft_strdup(const char *str)
{
	size_t	i;
	size_t	len;
	char	*d;

	len = ft_strlen(str);
	d = (char *)malloc(sizeof(char) * len +1);
	i = 0;
	if (!d)
		return (NULL);
	while (i < len)
	{
		d[i] = str[i];
		i++;
	}
	d[i] = '\0';
	return (d);
}

void	ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	unsigned int	i;

	if (!s || !f)
		return ;
	i = 0;
	while (s[i])
	{
		f(i, &s[i]);
		i++;
	}
}

int	ft_atoi(const char *str)
{
	int			signe;
	long long	tmp; 
	long long	result;

	result = 0;
	while ((*str >= 9 && *str <= 13) || *str == 32)
		str++;
	signe = 1;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			signe *= -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		tmp = result * 10 + (*str - 48);
		if (tmp < result && signe == 1)
			return (-1);
		if (tmp < result && signe == -1)
			return (0);
		result = tmp;
		str++;
	}
	return (result * signe);
}