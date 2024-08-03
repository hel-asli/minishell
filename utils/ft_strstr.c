/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 23:45:06 by hel-asli          #+#    #+#             */
/*   Updated: 2024/08/03 04:02:23 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Include/utils.h"

bool ft_strstr(char *str, char *del)
{
	size_t i = 0;

	while (str[i] && del[i] && str[i] == del[i])	
		i++;
	return (i == ft_strlen(del));
}