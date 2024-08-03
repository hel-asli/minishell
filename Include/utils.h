/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 23:52:32 by hel-asli          #+#    #+#             */
/*   Updated: 2024/08/03 04:02:42 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

// user headers

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>

/**			utils fcts			**/
char	**ft_split(char const *s);
char	**ft_free(char **split);
int		ft_atoi(const char *str);
size_t	ft_strlen(const char *str);
char	*ft_strdup(const char *str);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
int ft_strcmp(const char *s1, const char *s2);
void	ft_putstr_fd(char *s, int fd);
void	ft_putendl_fd(char *s, int fd);
void	err_handle(char *str);
bool	is_space(char c);
bool	empty_str(char *line);
bool	ft_strstr(char *str, char *del);



# endif