/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtok.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-feng <oel-feng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/24 03:25:41 by oel-feng          #+#    #+#             */
/*   Updated: 2024/08/24 03:32:37 by oel-feng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int is_delimiter(char s, char c)
{
    if (s == c)
            return 1;
    return 0;
}

char *ft_strtok(char *str, char c)
{
    static char *old = NULL;
    char *new;
	
    if (str == NULL)
        str = old;
    while (*str != '\0' && !is_delimiter(*str, c))
        str++;
    if (*str == '\0')
    {
        old = NULL;
        return NULL;
    }
    new = str;
    while (*str != '\0' && !is_delimiter(*str, c))
        str++;
    if (*str != '\0')
    {
        *str = '\0';
        old = str + 1;
    }
    else
        old = NULL;
    return new;
}