/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 20:01:46 by hel-asli          #+#    #+#             */
/*   Updated: 2024/09/20 17:27:26 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
char* del_quote(char *str)
{
    int     i;
    char    *ptr;
    char    type = '\0';
    bool    in_quote;
    bool    content_started = false;
    i = 0;
    int     j = 0;
    in_quote = false;
    ptr = malloc(sizeof(char) * strlen(str) + 1);
    while (str[i])
    {
        if (!in_quote && (str[i] == '\'' || str[i] == '"'))
        {
            if (content_started)
                ptr[j++] = str[i];
            in_quote = true;
            type = str[i];
        }
        else if (in_quote && type == str[i])
        {
            if (content_started)
                ptr[j++] = str[i];
            in_quote = false;
        }
        else
        {
            if (!content_started && str[i] != ' ')
                content_started = true;
            if (content_started)
                ptr[j++] = str[i];
        }
        i++;
    }
    while (j > 0 && ptr[j-1] == ' ')
        j--;
    ptr[j] = '\0';
    free(str);
    return (ptr);
}

int main(int ac, char **av)
{
    // int i = 0;
    // while (av[i])
    // {
    //     printf("{%s} ", av[i]);
    //     i++;
    // }
    // for (int i = 0; i < 5500; i++)
    //     printf("abc");
    // printf("\n");
    printf("%s\n", del_quote(strdup("   \"hello   world\"  ")));
}
