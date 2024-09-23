/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 04:45:33 by hel-asli          #+#    #+#             */
/*   Updated: 2024/08/10 05:24:37 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


// char	*ft_strdup(const char *str)
// {
// 	size_t	i;
// 	size_t	len;
// 	char	*d;

// 	len = ft_strlen(str);
// 	d = (char *)malloc(sizeof(char) * len +1);
// 	i = 0;
// 	if (!d)
// 		return (NULL);
// 	while (i < len)
// 	{
// 		d[i] = str[i];
// 		i++;
// 	}
// 	d[i] = '\0';
// 	return (d);
// }

char	*ft_strndup(const char *str, int index)
{
	int i;
	char *ptr;	

	i = 0;
	ptr = malloc(sizeof(char) * (index + 1));
	if (!str)
		return (NULL);

	while (str[i] && i < index)	
	{
		ptr[i] = str[i];
		i++;
	}
	ptr[i] = 0;
	return (ptr);
}
