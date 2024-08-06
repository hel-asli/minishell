/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_split.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 02:56:49 by hel-asli          #+#    #+#             */
/*   Updated: 2024/08/05 20:47:04 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char **ft_env_split(char *str)
{
	char **sp;
	int i = 0;
	int j = 0;

	sp = malloc(sizeof(char *) * 3);
	if (!sp)
		return (NULL);
	while (str[i] && str[i] != '=')
		i++;
	sp[0] = malloc(sizeof(char) * i + 1);
	if (!sp[0])
		return (NULL);
	i = 0;
	while (str[i] && str[i] != '=')
		sp[0][j++] = str[i++];
	sp[0][j] = '\0';
	if (str[i] == '=')		
		i++;
	j = 0;
	sp[1] = malloc(sizeof(char) * (ft_strlen(str) - i + 1));
	while (str[i])
		sp[1][j++] = str[i++];
	sp[1][j] = '\0';
	sp[2] = NULL;
	return (sp);
}