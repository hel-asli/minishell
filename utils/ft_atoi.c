/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 23:44:04 by hel-asli          #+#    #+#             */
/*   Updated: 2024/08/03 00:04:03 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Include/utils.h"

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
