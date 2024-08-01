/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 00:04:29 by hel-asli          #+#    #+#             */
/*   Updated: 2024/07/31 00:36:40 by hel-asli         ###   ########.fr       */
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