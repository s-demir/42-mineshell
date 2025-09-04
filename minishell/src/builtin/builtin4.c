/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin4.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amsagman <amsagman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 12:20:21 by amsagman          #+#    #+#             */
/*   Updated: 2025/08/24 12:20:24 by amsagman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*handle_parent_dir(char *cur_pwd, int cur_len)
{
	char	*resolved;
	int		i;

	i = cur_len - 1;
	while (i >= 0 && cur_pwd[i] != '/')
		i--;
	if (i > 0)
	{
		resolved = galloc(i + 1);
		if (resolved)
			duplicate_string(resolved, cur_pwd, 0, i);
	}
	else
	{
		resolved = galloc(2);
		if (resolved)
		{
			resolved[0] = '/';
			resolved[1] = '\0';
		}
	}
	return (resolved);
}

static char	*append_path(char *cur_pwd, char *path, int cur_len, int path_len)
{
	char	*resolved;

	resolved = galloc(cur_len + path_len + 2);
	if (!resolved)
		return (NULL);
	duplicate_string(resolved, cur_pwd, 0, cur_len);
	if (cur_pwd[cur_len - 1] != '/')
	{
		resolved[cur_len] = '/';
		duplicate_string(resolved + cur_len + 1, path, 0, path_len);
	}
	else
		duplicate_string(resolved + cur_len, path, 0, path_len);
	return (resolved);
}

char	*resolve_path(char *cur_pwd, char *path)
{
	int	cur_len;
	int	path_len;

	if (!cur_pwd || !path)
		return (NULL);
	if (path[0] == '/')
		return (strcopy(path));
	cur_len = string_length(cur_pwd, '\0');
	path_len = string_length(path, '\0');
	if (str_cmp(path, "..", NULL))
		return (handle_parent_dir(cur_pwd, cur_len));
	else if (str_cmp(path, ".", NULL))
		return (strcopy(cur_pwd));
	return (append_path(cur_pwd, path, cur_len, path_len));
}

char	*get_logical_pwd(t_env *env)
{
	int	pwd_idx;

	if (env->logical_pwd)
		return (env->logical_pwd);
	pwd_idx = locate_env_var_index(env, "PWD");
	if (pwd_idx >= 0)
		return (env->parsed_env[pwd_idx][1]);
	return (NULL);
}

void	update_logical_pwd(t_env *env, char *new_path)
{
	if (env->logical_pwd)
		env->logical_pwd = NULL;
	env->logical_pwd = strcopy(new_path);
}
