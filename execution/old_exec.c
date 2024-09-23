/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   old_exec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-feng <oel-feng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/24 03:52:24 by oel-feng          #+#    #+#             */
/*   Updated: 2024/08/24 03:52:47 by oel-feng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "../minishell.h"

// static char	*env_strjoin(char *s1, char *s2)
// {
// 	char	*ptr;
// 	int		i;
// 	int		j;

// 	if (!s1)
// 		return (ft_strdup(s2));
// 	ptr = malloc(sizeof(char) * ft_strlen(s1) + ft_strlen(s2) + 1);
// 	i = 0;
// 	j = 0;

// 	while (s1[i])
// 	{
// 		ptr[i] = s1[i];
// 		i++;
// 	}
// 	while (s2[j])
// 		ptr[i++] = s2[j++];
// 	ptr[i] = '\0';
// 	free(s1);
// 	return (ptr);
// }

// char **convert_env(t_env *env)
// {
// 	t_env	*curr;
// 	char	**envp;
// 	int		i;

// 	i = 0;
// 	curr = env;
// 	while (curr)
// 	{
// 		i++;
// 		curr = curr->next;
// 	}
// 	envp = malloc(sizeof(char *) * (i + 1));
// 	i = 0;
// 	curr = env;
// 	while (curr)
// 	{
// 		envp[i] = env_strjoin(curr->key, "=");
// 		envp[i] = env_strjoin(envp[i], curr->value);
// 		i++;
// 		curr = curr->next;
// 	}
// 	envp[i] = NULL;
// 	return (envp);
// }

// void	execution(t_shell **shell, char **env)
// {
// 	t_shell		*tmp;
// 	t_commands	*curr;

// 	tmp = *shell;
// 	curr = tmp->commands;
// 	while (curr)
// 	{
// 		if (curr->args)
// 			execve(curr->cmd, curr->args, env);
// 		else
// 			execve(curr->cmd, NULL, env);
// 		curr = curr->next;
// 	}
// }

// int execute(t_commands **cmds, char **ev, int tmp)
// {
//     t_commands *curr = *cmds;
//     int pip = 0;
//     int pid;
//     int fd[2];

//     if (curr->next)
//         pip = 1;
    
//     if (pip && pipe(fd) == -1)
//         return (ft_putstr_fd("Error\n", 2), 1);
    
//     pid = fork();
//     if (!pid)
//     {
//         if (dup2(tmp, 0) == -1 || close(tmp) == -1)
//             return (ft_putstr_fd("Error 1\n", 2), 1);
//         if (pip && (dup2(fd[1], 1) == -1 || close(fd[1]) == -1 || close(fd[0]) == -1))
//             return (ft_putstr_fd("Error 2\n", 2), 1);
//         execve(curr->cmd, curr->args, ev);
//         return (ft_putstr_fd("Error 3 ", 2), ft_putstr_fd(curr->cmd, 2), ft_putstr_fd("\n", 2), 1);
//     }
    
//     if (pip && (dup2(fd[0], tmp) == -1 || close(fd[0]) == -1 || close(fd[1]) == -1))
//         return (ft_putstr_fd("Error4\n", 2), 1);
//     if (!pip && (dup2(0, tmp) == -1 || waitpid(pid, NULL, 0) == -1))
//         return (ft_putstr_fd("Error5\n", 2), 1);
//     *cmds = curr->next;
    
//     return 0;
// }

// void execution_start(t_shell *shell, char **ev)
// {
//     int tmp;
    
//     if (builtins_check(&shell->commands, &shell->env, &shell->export))
//         return;
//     else {
//         tmp = dup(0);
//         while (shell->commands)
//         {
//             if (execute(&shell->commands, ev, tmp) != 0)
//                 break;
//         }
//         close(tmp);
//     }
// 	// {
// 	// 	if (env)
// 	// 		ft_free(env);
// 	// 	env = convert_env(shell->env);
// 	// 	execution(&shell, env);
// 	// 	ft_free(env);
// 	// }
// }
