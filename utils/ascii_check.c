/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ascii_check.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 02:36:07 by oel-feng          #+#    #+#             */
/*   Updated: 2024/09/23 02:54:20 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool    ft_isalpha(int c)
{
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
        return (true);
    return (false);
}


bool	ft_isdigit(int c)
{
    if (c >= '0' && c <= '9')
        return (true);
    return (false);
}

bool is_ascii(char *str)
{
    int i = 0;

    while (str[i])
    {
        if (str[i] < 0)
            return (false);
        i++;
    }
    return (true);
}

bool    ft_isalnum(int c)
{
    if (ft_isalpha(c) || ft_isdigit(c))
        return (true);
    return (false);
}
