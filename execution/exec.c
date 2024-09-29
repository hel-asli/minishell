/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-feng <oel-feng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 00:49:12 by oel-feng          #+#    #+#             */
/*   Updated: 2024/09/29 15:54:03 by oel-feng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Debug Code 
char *find_command(char *cmd, char **ev)
{
    char *path;
    char *dir;
    char *full_path;
    char *path_copy;

    if (cmd == NULL)
        return NULL;

    if (my_strchr_v2(cmd, '/') != NULL) {
        if (access(cmd, X_OK) == 0)
            return ft_strdup(cmd);
        else
            return (ft_fprintf(2, "Error: Command not found: %s\n", cmd), NULL);
    }
    char *standard_paths[] = {"/usr/bin/", "/bin/"};
    for (int i = 0; i < 2; i++) {
        full_path = malloc(strlen(standard_paths[i]) + strlen(cmd) + 1);
        if (!full_path)
            return NULL;
        full_path = non_free_strjoin(standard_paths[i], cmd);
        if (access(full_path, X_OK) == 0) {
            return full_path;
        }
        free(full_path);
    }
    for (int i = 0; ev[i]; i++) {
        if (ft_strncmp(ev[i], "PATH=", 5) == 0) {
            path = ev[i] + 5;
            path_copy = ft_strdup(path);
            if (!path_copy)
                return NULL;
            dir = ft_strtok(path_copy, ':');
            while (dir) {
                full_path = malloc(strlen(dir) + strlen(cmd) + 2);
                if (!full_path) {
                    free(path_copy);
                    return NULL;
                }
                full_path = non_free_strjoin(dir, "/");
                full_path = non_free_strjoin(full_path, cmd);
                if (access(full_path, X_OK) == 0) {
                    free(path_copy);
                    return full_path;
                }
                free(full_path);
                dir = ft_strtok(NULL, ':');
            }
            free(path_copy);
            break;
        }
    }
    return NULL;
}

int execute(t_commands **cmds, char **ev, int *tmp)
{
    t_commands *curr = *cmds;
    int pip = 0;
    int pid;
    int fd[2];
    char *cmd_path;
    int status;

    while (curr)
    {
        pip = (curr->next != NULL);
        if (pip && pipe(fd) == -1)
            return (ft_putstr_fd("Error: pipe failed\n", 2), 1);
        pid = fork();
        if (pid == -1)
            return (ft_putstr_fd("Error: fork failed\n", 2), 1);
        if (pid == 0)
        {
            if (curr->redirect)
                handle_redirections(curr->redirect);
            if (dup2(*tmp, 0) == -1 || close(*tmp) == -1)
                return (ft_putstr_fd("Error 1: dup2 or close failed\n", 2), 1);
            if (pip && (dup2(fd[1], 1) == -1 || close(fd[1]) == -1 || close(fd[0]) == -1))
                return (ft_putstr_fd("Error 2: dup2 or close failed for pipe\n", 2), 1);
            cmd_path = find_command(curr->cmd, ev);
            if (cmd_path == NULL)
            {
                ft_fprintf(2, "Error: Command not found: %s\n", curr->cmd);
                exit(1);
            }
            if (execve(cmd_path, curr->args, ev) == -1)
            {
                ft_fprintf(2, "Error: %s: %s\n", curr->cmd, strerror(errno));
                free(cmd_path);
                exit(1);
            }
            free(cmd_path);
            exit(1);
        }
        if (close(*tmp) == -1)
            return (ft_putstr_fd("Error 3: close failed\n", 2), 1);
        if (pip)
        {
            *tmp = fd[0];
            if (close(fd[1]) == -1)
                return (ft_putstr_fd("Error 4: close failed for pipe\n", 2), 1);
        }
        else
        {
            *tmp = dup(0);
            if (*tmp == -1)
                return (ft_putstr_fd("Error 5: dup failed\n", 2), 1);
        }
        curr = curr->next;
    }
    while (wait(&status) > 0);

    *cmds = NULL;
    return 0;
}
