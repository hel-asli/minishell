/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 23:08:12 by hel-asli          #+#    #+#             */
/*   Updated: 2024/08/02 02:31:17 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <limits.h>
# include <sys/errno.h>
# include <stdbool.h>
# include <readline/readline.h>
# include <readline/history.h>

/**			utils fcts			**/
char	**ft_split(char const *s);
int		ft_atoi(const char *str);
size_t	ft_strlen(const char *str);
char	*ft_strdup(const char *str);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
int ft_strcmp(const char *s1, const char *s2);
void	ft_putstr_fd(char *s, int fd);
void	ft_putendl_fd(char *s, int fd);

#endif