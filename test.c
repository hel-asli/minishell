/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 20:01:46 by hel-asli          #+#    #+#             */
/*   Updated: 2024/10/17 23:18:48 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/ioctl.h>
#define RL_S "\1"
#define RL_E "\2"
volatile int n = 0;
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

void sigint_func(int nb)
{
    (void)nb;
    n = 1;
    ioctl(STDIN_FILENO, TIOCSTI, "\n");
    rl_replace_line("", 0);
}

char	**ft_free(char **split)
{
	int	i;

	i = 0;
    if (split)
    {
        while (split[i])
        {
            free(split[i]);
            i++;
        }
        free(split);
    }
	return (NULL);
}
void disable_newline_behavior() {
    struct termios term;
    tcgetattr(STDOUT_FILENO, &term);
    
    // Modify terminal attributes to disable automatic newline handling
    term.c_oflag |= ONLCR;  // Disable carriage return to newline conversion
    tcsetattr(STDOUT_FILENO, TCSANOW, &term);
}
int main(int ac, char **av)
{
    // // int i = 0;
    // // while (av[i])
    // // {
    // //     printf("{%s} ", av[i]);
    // //     i++;
    // // }
    // // for (int i = 0; i < 5500; i++)
    // //     printf("abc");
    // // printf("\n");
    // printf("%s\n", del_quote(strdup("   \"hello   world\"  ")));
    (void)ac;
    (void)av;
    char *line;
    // int a = 0;
    // pid_t id = fork();

    // if (id == 0)
    // {
    //     fprintf(stderr, "before : %p\n", &a);
    //     a = 1;
    //     fprintf(stderr, "before : %p\n", &a);
    // }
    // else
    // {
    //     waitpid(id, NULL, 0);
    // }

    rl_initialize();
    // signal(SIGINT, sigint_func);
    // disable_newline_behavior();
    while (true)
    {
        line = readline("> ");
        if (!line)
        {
            // rl_replace_line("", 0);
            // rl_clear_visible_line();
            // ft_free(ev);
            // ev = NULL;
            // fflush(stdout);
            // rl_clear_visible_line(); 
            rl_crlf();
            printf("exit\n");
            break;
        }
    }
    // line = readline("hoho :");
}
