/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 17:55:49 by oel-feng          #+#    #+#             */
/*   Updated: 2024/10/19 03:18:11 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../minishell.h"

static void    export_print(t_env **exp)
{
	t_env	*export;

	export = *exp;
	while (export)
	{
		if (export->value)
			printf("declare -x %s=\"%s\"\n", export->key, export->value);
		else
			printf("declare -x %s\n", export->key);
		export = export->next;
	}
}

static char	*copy_key(char *src)
{
	int		len;
	int		i;
	char	*dest;

	i = 0;
	len = 0;
	while (src[len] && (src[len] != '=' && src[len] != '+'))
		len++;
	dest = (char *)malloc(sizeof(char) * (len + 1));
	while (i < len)
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

static void	sort_export(t_env **export)
{
	t_env		*curr;
	t_env		*tmp;
	char		*key;
	char		*value;

	curr = *export;
	while (curr)
	{
		tmp = curr->next;
		while (tmp)
		{
			if (curr->key && ft_strcmp(curr->key, tmp->key) > 0)
			{
				key = curr->key;
				value = curr->value;
				curr->key = tmp->key;
				curr->value = tmp->value;
				tmp->key = key;
				tmp->value = value;
			}
			tmp = tmp->next;
		}
		curr = curr->next;
	}
	export_print(export);
}

static bool	env_key_exist(t_env **env, char *key)
{
	t_env	*curr;

	curr = *env;
	while (curr)
	{
		if (!ft_strcmp(curr->key, key))
			return (true);
		curr = curr->next;
	}
	return (false);
}

static bool is_valid_export(char *str)
{
	int	i;

	i = 0;
	if (!ft_isalpha(str[i]) && str[i] != '_')
		return (false);
	i++;
	while (str[i])
	{
		if (str[i] == '+' && str[i + 1] == '=')
			break;
		if (str[i] == '=')
			break;
		if (str[i] == '+' && str[i + 1] != '=')
			return (false);
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (false);
		i++;
	}
	return (true);	
}


static void	env_concat(t_env **env, char *key, char *value)
{
	t_env	*curr;

	curr = *env;
	while (curr)
	{
		if (!ft_strcmp(curr->key, key))
		{
			curr->value = ft_strjoin(curr->value, value);
			return ;
		}
		curr = curr->next;
	}
}

static void	env_export(t_env **env, char *key, char *value)
{
	t_env *curr;

	curr= *env;
	while (curr)
	{
		if (!ft_strcmp(curr->key, key))
		{
			free(curr->value);
			curr->value = ft_strdup(value);
			free(value);
			return ;
		}
		curr = curr->next;
	}
}

static int	num_count(char *key)
{
	int i;
	int	num;

	i = 0;
	num = 0;
	while (key[i] && key[i] != '=')
	{
		if (key[i] == '+')
			num++;
		i++;
	}
	return (num);
}
// static char	*ft_quotes(char* org)
// {
//     int		i = 0;
// 	int	 	j = 0;
//     int 	len;
//     char	*cpy;
	
// 	len = ft_strlen(org);
// 	cpy = (char *)malloc(sizeof(char) * (len + 1));
//     while (i < len)
// 	{
//         if (org[i] != '"' && org[i] != '\'')
// 		{
//             cpy[j] = org[i];
// 			j++;
// 		}
// 		i++;
//     }
//     cpy[j] = '\0';
//     return (cpy);
// }

static void export_handler(t_env **export, char *args)
{
	char	**key;
	char	*new_key;
	
	key = ft_env_split(args);
	if (num_count(key[0]) > 1)
	{
		ft_fprintf(2, "minishell: export: `%s': not a valid identifier\n", key[0]);
		ft_free(key);
		return ;
	}
	new_key = copy_key(key[0]);
	if (env_key_exist(export, new_key))
	{
		if (ft_lookup(args, '+'))
			env_concat(export, new_key, ft_strdup(key[1]));
		else if (ft_lookup(args, '='))
			env_export(export, new_key, ft_strdup(key[1]));
		else if (!ft_lookup(args, '='))
			env_export(export, new_key, ft_strdup("\0"));
		free(new_key);
	}
	else
	{
		if (ft_lookup(args, '+'))
			ft_lstadd_back(export,ft_lstnew(new_key, ft_strdup(key[1])));
		else if (ft_lookup(args, '='))
			ft_lstadd_back(export,ft_lstnew(new_key, ft_strdup(key[1])));
		else if (!ft_lookup(args, '='))
			ft_lstadd_back(export,ft_lstnew(new_key, NULL));	
	}
	fr_args(key);
}	

bool	my_export(t_commands *cmnds, t_shell *shell, t_env **env, int flag)
{
	t_env		**export;
	t_commands	*curr;
	int			i;

	i = 1;
	curr = cmnds;
	export = env;

	if (curr->redirect && !flag && handle_redirections(curr->redirect))
		return (shell->exit_status = EXIT_FAILURE, true);
	if (!curr->args[i])
		return(sort_export(export), true);
	while (curr->args[i])
	{
		if (!is_valid_export(curr->args[i]))
		{
			ft_fprintf(2, "minishell: export: `%s': not a valid identifier\n", curr->args[i]);
			shell->exit_status = EXIT_FAILURE;
			return (true);
		}
		export_handler(export, curr->args[i]);
		i++;
	}

	env = export;
	shell->exit_status = EXIT_SUCCESS;
	return (true);
}