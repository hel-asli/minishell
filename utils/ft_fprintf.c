/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-feng <oel-feng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 21:24:22 by oel-feng          #+#    #+#             */
/*   Updated: 2024/08/25 21:26:59 by oel-feng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	ft_putchar(char c, int *size, int fd)
{
	(*size)++;
	write(fd, &c, 1);
}

static void	ft_putnbr(int nbr, int *size, int fd)
{
	long	n;

	n = nbr;
	if (n < 0)
	{
		ft_putchar('-', size, fd);
		n *= -1;
	}
	if (n > 9)
	{
		ft_putnbr(n / 10, size, fd);
		ft_putchar((n % 10) + 48, size, fd);
	}
	else
		ft_putchar(n + 48, size, fd);
}

static void	ft_putstr(char *str, int *size, int fd)
{
	int	i;

	i = 0;
	if (!str)
	{
		ft_putstr("(null)", size, fd);
		return ;
	}
	while (str[i])
		ft_putchar(str[i++], size, fd);
}

static void	ft_recognition(va_list args, const char format, int *size, int fd)
{
	if (format == 'c')
		ft_putchar(va_arg(args, int), size, fd);
	else if (format == 's')
		ft_putstr(va_arg(args, char *), size, fd);
	else if (format == 'd' || format == 'i')
		ft_putnbr(va_arg(args, int), size, fd);
	else
		ft_putchar(format, size, fd);
}

int	ft_fprintf(int fd, const char *format, ...)
{
	int		size;
	int		i;
	va_list	args;

	if (write(fd, "", 0) == -1)
		return (-1);
	size = 0;
	i = 0;
	va_start(args, format);
	while (format[i])
	{
		if (format[i] == '%')
		{
			i++;
			if (format[i] == '\0')
				break ;
			ft_recognition(args, format[i], &size, fd);
		}
		else
			ft_putchar(format[i], &size, fd);
		i++;
	}
	va_end(args);
	return (size);
}
