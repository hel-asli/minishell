/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 00:49:12 by oel-feng          #+#    #+#             */
/*   Updated: 2024/10/18 00:30:48 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// char *find_command(char *cmd, char **ev)
// {
//     char *path;
//     char *dir;
//     char *full_path;
//     char *path_copy;

//     if (cmd == NULL)
//         return NULL;
//     if (my_strchr_v2(cmd, '/') != NULL) {
//         if (access(cmd, X_OK) == 0)
//             return ft_strdup(cmd);
//         else
//             return (ft_fprintf(2, "Error: Command not found: %s\n", cmd), NULL);
//     }
//     for (int i = 0; ev[i]; i++) {
//         if (ft_strncmp(ev[i], "PATH=", 5) == 0) {
//             path = ev[i] + 5;
//             path_copy = ft_strdup(path);
//             if (!path_copy)
//                 return NULL;
//             dir = ft_strtok(path_copy, ':');
//             while (dir) {
//                 full_path = malloc(strlen(dir) + strlen(cmd) + 2);
//                 if (!full_path) {
//                     free(path_copy);
//                     return NULL;
//                 }
//                 ft_strcpy(full_path, dir);
//                 ft_strcat(full_path, "/");
//                 ft_strcat(full_path, cmd);
//                 if (access(full_path, X_OK) == 0) {
//                     free(path_copy);
//                     return full_path;
//                 }
//                 free(full_path);
//                 dir = ft_strtok(NULL, ':');
//             }
//             free(path_copy);
//             break;
//         }
//     }
//     return NULL;
// }

char *find_command(char *cmd, t_env *env)
{
    char *full_path;
	int k;
	t_env *tmp;
	char **pt;

	tmp = env;
    if (!cmd || !cmd[0])
        return NULL;
    if (my_strchr_v2(cmd, '/') != NULL)
	{
        if (access(cmd, X_OK) == 0)
            return ft_strdup(cmd);
        else
            return (NULL);
    }
	pt = NULL;
	k = 0;
	full_path = NULL;
	while (tmp)
	{	
		if (!ft_strcmp(tmp->key, "PATH"))
		{
			pt = ft_split_v2(tmp->value, ':');
			if (!pt)
				err_handle("split");
			while (pt[k])
			{
				full_path = NULL;
				full_path = ft_strjoin_char(pt[k], cmd, '/');
				if (access(full_path, X_OK) == 0)
					return (ft_free(pt), full_path);
				free(full_path);
				k++;
			}
			ft_free(pt);
		}
		tmp = tmp->next;
	}
    return NULL;
}

// int execute(t_shell *shell, t_commands **cmds, char **ev, int *tmp)
// {
//     t_commands *curr = *cmds;
//     int pip = 0;
//     int pid;
//     int fd[2];
//     char *cmd_path;
//     int status;

//     while (curr)
//     {
//         pip = (curr->next != NULL);
//         if (pip && pipe(fd) == -1)
//             return (ft_putstr_fd("Error: pipe failed\n", 2), 1);
//         if (curr->cmd && builtins_check(&curr, &shell->env, &shell->export))
//         {
//             if (dup2(*tmp, STDIN_FILENO) == -1)
//                 return (ft_putstr_fd("Error: dup2 failed\n", 2), 1);
//             if (pip && dup2(fd[1], STDOUT_FILENO) == -1)
//                 return (ft_putstr_fd("Error: dup2 for pipe failed\n", 2), 1);
//             if (fd[1] && close(fd[1]) == -1)
//                 return (ft_putstr_fd("Error: close failed 2222222222\n", 2), 1);
// 			close(fd[0]);
//             curr = curr->next;
//             continue;
//         }

//         pid = fork();
//         if (pid == -1)
//             return (ft_putstr_fd("Error: fork failed\n", 2), 1);
        
//         if (pid == 0)
//         {
//             if (curr->redirect)
//                 handle_redirections(curr->redirect);

//             cmd_path = find_command(curr->cmd, shell->env);
//             if (cmd_path == NULL)
//             {
//                 ft_fprintf(2, "Error: Command not found: %s\n", curr->cmd);
//                 exit(1);
//             }
//             if (execve(cmd_path, curr->args, ev) == -1)
//             {
//                 ft_fprintf(2, "Error: %s: %s\n", curr->cmd, strerror(errno));
//                 free(cmd_path);
//                 exit(1);
//             }
//             free(cmd_path);
//             exit(0);
//         }
//         if (close(*tmp) == -1)
//             return (ft_putstr_fd("Error 3: close failed\n", 2), 1);
//         if (pip)
//         {
//             *tmp = fd[0];
//             if (close(fd[1]) == -1)
//                 return (ft_putstr_fd("Error 4: close failed for pipe\n", 2), 1);
//         }
//         else
//         {
//             *tmp = dup(STDIN_FILENO);
//             if (*tmp == -1)
//                 return (ft_putstr_fd("Error 5: dup failed\n", 2), 1);
//         }
//         curr = curr->next;
//     }
//     while (wait(&status) > 0);
//     *cmds = NULL;
//     return 0;
// }



// char *standard_paths[] = {"/usr/bin/", "/bin/"};
// for (int i = 0; i < 2; i++) {
//     full_path = malloc(ft_strlen(standard_paths[i]) + ft_strlen(cmd) + 1);
//     if (!full_path)
//         return NULL;
//     full_path = non_free_strjoin(standard_paths[i], cmd);
//     if (access(full_path, X_OK) == 0) {
//         return full_path;
//     }
//     free(full_path);
// }
// for (int i = 0; ev[i]; i++) {
//     if (ft_strncmp(ev[i], "PATH", 4) == 0) {
//         path = ev[i] + 5;
//         path_copy = ft_strdup(path);
//         if (!path_copy)
//             return NULL;
//         dir = ft_strtok(path_copy, ':');
//         while (dir) {
//             full_path = malloc(strlen(dir) + ft_strlen(cmd) + 2);
//             if (!full_path) {
//                 free(path_copy);
//                 return NULL;
//             }
//           +
//             if (access(full_path, X_OK) == 0) {
//                 free(path_copy);
//                 return full_path;
//             }
//             free(full_path);
//             dir = ft_strtok(NULL, ':');
//         }
//         free(path_copy);
//         break;
//     }
// }