/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_helper.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-feng <oel-feng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 09:25:05 by oel-feng          #+#    #+#             */
/*   Updated: 2024/10/23 10:04:27 by oel-feng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	update_pwd(t_env **env, t_env *tmp, char *pwd)
{
	if (is_exists("PWD", tmp))
		env_update(env, "PWD", pwd);
	else
		ft_lstadd_back(env, ft_lstnew(ft_strdup("PWD"),
				ft_strdup(pwd), 0));
	return (true);
}

bool	update_oldpwd(t_env **env, t_env *tmp, char *oldpwd)
{
	if (!oldpwd)
		return (false);
	if (!is_exists("OLDPWD", tmp))
		ft_lstadd_back(env, ft_lstnew(ft_strdup("OLDPWD"),
				ft_strdup(oldpwd), 0));
	else
		env_update(env, "OLDPWD", oldpwd);
	return (true);
}

bool	cd_home(t_env **env, char *oldpwd, int *status)
{
	t_env	*tmp;
	char	*home;
	char	*pwd;

	tmp = *env;
	home = get_env("HOME", tmp);
	if (!home)
	{
		ft_putstr_fd("minishell: HOME not set\n", STDERR_FILENO);
		*status = EXIT_FAILURE;
		return (free(oldpwd), true);
	}
	if (chdir(home) < 0)
	{
		*status = EXIT_FAILURE;
		perror("HOME");
		return (free(oldpwd), free(home), true);
	}
	pwd = getcwd(NULL, 0);
	if (!pwd)
		return (perror("getcwd"), free(home), free(oldpwd), true);
	update_oldpwd(env, tmp, oldpwd);
	update_pwd(env, tmp, pwd);
	return (free(pwd), free(oldpwd), free(home), true);
}

bool	cd_path(t_env **env, char *path, char *oldpwd, int *status)
{
	char	*pwd;
	t_env	*tmp;

	tmp = *env;
	if (chdir(path) == -1)
		return (*status = EXIT_FAILURE, perror("cd"), free(oldpwd), true);
	pwd = getcwd(NULL, 0);
	if (!pwd && (ft_strcmp(path, "..") || ft_strcmp(path, ".")))
	{
		ft_fprintf(STDERR_FILENO, "cd: error retrieving current directory: ");
		ft_fprintf(STDERR_FILENO, "getcwd: cannot access parent directories: ");
		ft_fprintf(STDERR_FILENO, "No such file or directory\n");
		pwd = ft_strjoin_char(oldpwd, path, '/');
		*status = EXIT_FAILURE;
	}
	else if (!pwd)
		return (*status = EXIT_FAILURE, perror("getcwd"), free(oldpwd), true);
	update_oldpwd(env, tmp, oldpwd);
	update_pwd(env, tmp, pwd);
	*status = 0;
	return (free(pwd), free(oldpwd), true);
}
