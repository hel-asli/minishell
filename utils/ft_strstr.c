/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 23:45:06 by hel-asli          #+#    #+#             */
/*   Updated: 2024/08/05 20:46:36 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool ft_strstr(char *str, char *del)
{
	size_t i = 0;

	while (str[i] && del[i] && str[i] == del[i])	
		i++;
	return (i == ft_strlen(del));
}