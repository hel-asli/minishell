/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 20:01:46 by hel-asli          #+#    #+#             */
/*   Updated: 2024/10/20 03:43:36 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <sys/ioctl.h>
#include <sys/types.h>
#include <dirent.h>

// void ft_free(char **split)
// {
// 	int	i;

// 	i = 0;
//     if (split)
//     {
//         while (split[i])
//         {
//             free(split[i]);
//             i++;
//         }
//         free(split);
//     }
// }
bool	ft_strchr(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (true);
		i++;
	}
	return (false);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] != '\0' && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}

size_t arr_len(char **tab)
{
	size_t i;

	i = 0;
	if (!tab)
		return (0);
	while (tab[i])
		i++;
	return (i);
}

size_t	ft_strlen(const char *str)
{
	size_t	len;

	len = 0;
	while (str[len])
		len++;
	return (len);
}

void fr_args(char **args)
{
	int i = -1;

	if (args)
	{
		while (args[++i])
			free(args[i]);
		free(args);
	}
}

char **add_arr(char **args, char *str)
{
	size_t len;

	len = arr_len(args);
	char **new = malloc((sizeof(char *) * (len + 2)));
	if (!new)
		return (NULL);
	size_t i = 0;
	while (i < len && args)
	{
		new[i] = ft_strdup(args[i]);
		i++;
	}
	if (str)
		new[i++] = ft_strdup(str);
	new[i] = NULL;
	fr_args(args);
	return (new);
}

void	ft_putendl_fd(char *s, int fd)
{
	if (s)
	{
		while (*s)
			write(fd, s++, 1);
		write(fd, "\n", 1);
	}
}

void	err_handle(char *str)
{
	ft_putendl_fd(str, STDERR_FILENO);
	exit(EXIT_FAILURE);
}


bool	is_rev_special(char c)
{
	return (c == 1 || c == 2 || c == 3 || c == 4 || (c >= 15 && c <= 19));
}

void gar_protect(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (is_rev_special(str[i]))
			match_rev(str, i);
		i++;
	}
}

void	match_rev(char *line, int i)
{
	if (line)
	{
		if (line[i] == 1)
			line[i] = '|';
		else if (line[i] == 2)
			line[i] = '<';
		else if (line[i] == 3)
			line[i] = '>';
		else if (line[i] == 4)
			line[i] = 32;
		else if (line[i] >= 15 && line[i] <= 19)
			line[i] -= 6;
	}
}

char	**re_build_arg(char **args, char **sp)
{
	size_t len;
    len = arr_len(args) + arr_len(sp);
    int j = 0;
    int k = 0;
    char **ptr = malloc(sizeof(char *) * (len + 1)); 
    if (!ptr)
        err_handle("malloc");
    while (args && args[j])
    {
        ptr[j] = ft_strdup(args[j]);
        j++;
    }
    while (sp[k])
    {
		// gar_protect(sp[k]);
     	ptr[j++] = ft_strdup(sp[k]);
        k++;
    }
    ptr[j] = NULL;
	fr_args(args);
    return (ptr);
}

char	*ft_strdup(const char *str)
{
	size_t	i;
	size_t	len;
	char	*d;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	d = (char *)malloc(sizeof(char) * len +1);
	i = 0;
	if (!d)
		return (NULL);
	while (i < len)
	{
		d[i] = str[i];
		i++;
	}
	d[i] = '\0';
	return (d);
}

char *str_add_char(char *str, char c)
{
	int i = 0;
	char *ptr = malloc(sizeof(char) * (ft_strlen(str) + 2));
	if (!ptr)
		return (NULL);
	while (str[i])
	{
		ptr[i] = str[i];
		i++;
	}
	ptr[i++] = c;
	ptr[i] = '\0';
	free(str);
	return (ptr);
}
int starts_with(char *start, char *str)
{
    int i = 0;

    while (str[i] && start[i])
    {
        if (str[i] == start[i])
            i++;
        else
            break;
    }
    if (!str[i])
        return (0);
    return (1);
}

int check_wildcard(char *str)
{
    int i;
    bool db = false;
    bool si = false;

    i = 0;
    while (str[i])
    {
        if (str[i] == '\'' && !db)
            si = !si;
        else if (str[i] == '"' && !si)
            db = !db;
        else if ((db || si) && str[i] == '*')
            return (false);
        i++;
    }
    return (true);
}

bool dir_check(char *str)
{
    int i = 0;
    bool test = false;
    while (str[i])
    {
        if (str[i] == '*')
            test = true;
        if (test && str[i] == '/')
            return (true);
        i++;
    }
    return (false);
}


// hell****.txt
// hellworladfadfadsfas.txt
// bool check_pattern(char *pattern, char *str)
// {
//     int i = 0;
//     int j = 0;
//     while (str[j] && pattern[i])
//     {
//         if (pattern[i] == '*')
//         {
//             while(pattern[i] == '*')
//                 i++;
//             if (pattern[i] == 0 && j == 0)
//                 return (true);
//         }
//         else if (str[j] == pattern[i])
//         {
//             i++;
//             j++;
//         }
//         else
//             return (false);
//     }
//     while (pattern[i] && pattern[i] == '*')
//         j++;
//     return (!pattern[i] && !str[j]);
// }


char	*ft_strjoin(char *s1, char *s2)
{
	char	*ptr;
	int		i;
	int		j;

	if (!s1)
		return (ptr = ft_strdup(s2), free(s2), ptr);
	ptr = malloc(sizeof(char) * ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!ptr)
		err_handle("Malloc Failure");
	i = 0;
	j = 0;
	while (s1[i])
	{
		ptr[i] = s1[i];
		i++;
	}
	while (s2[j])
		ptr[i++] = s2[j++];
	ptr[i] = '\0';
	free(s1);
	free(s2);
	return (ptr);
}

char **sub_dire(char *str, char *prefix, char *old, DIR *sub)
{
    struct dirent *fsub;
    char **tab;

    fsub = readdir(sub);
    tab = NULL;
    while (fsub)
    {
        if (ft_strcmp(fsub->d_name, ".") &&  ft_strcmp(fsub->d_name, ".."))
        {
            if (check_pattern(str, fsub->d_name))
                tab = add_arr(tab, ft_strjoin(ft_strdup(prefix), ft_strdup(fsub->d_name)));
        }
        fsub = readdir(sub);
    }
    return (tab);
}
int main(int ac, char **av)
{
        DIR *dir;
        char *pwd;
        char **tab;
        char *prefix;
        pwd = getcwd(NULL, 0);
        if (!pwd)
        {
            perror("getcwd");
            return (1);
        }
        tab = NULL;
        prefix = ft_strdup("");
        dir = opendir(".");
        if (!dir)
        {
            perror("opendir");
            return (1);
        }
        char *str = "/Users/hel-asli/minishell/a/*.txt";
        if (ft_strchr(str, '/') && is_not_sub(str, pwd))
            printf("add normaly to tab");
        else if (ft_strchr(str, '*') && check_wildcard(str))
        {
            tab = get_files(str, prefix, dir);
        }
        else
            fprintf(stderr, "just %s\n", str);

        int k = 0;
        while (tab && tab[k])
        {
            fprintf(stderr, "%s\n", tab[k]);
            k++;
        }
        closedir(dir);
        return (0);
}
