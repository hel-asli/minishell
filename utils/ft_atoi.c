/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 23:44:04 by hel-asli          #+#    #+#             */
/*   Updated: 2024/10/21 03:22:54 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_atoi(char *str)
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

long long	ft_exit_atol(t_shell *shell, char *str)
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
			exit_error(shell, 1);
		if (tmp < result && signe == -1)
			exit_error(shell, 1);
		result = tmp;
		str++;
	}
	return (result * signe);
}
