/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amsagman <amsagman@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 16:49:35 by sedemir           #+#    #+#             */
/*   Updated: 2025/08/14 00:32:58 by amsagman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*check_path_without_env(char *file, int mode)
{
	char	*tmp_path;
	int		b;

	b = string_length(file, ' ');
	tmp_path = galloc(b + 1);
	if (!tmp_path)
		return (NULL);
	duplicate_string(tmp_path, file, 0, b);
	if (check_path_access(tmp_path, mode))
		return (tmp_path);
	return (NULL);
}

char	*check_env_path(char *env_path, char *file, int *indices, int mode)
{
	char	*path;

	path = build_path_from_env(env_path, file, indices);
	if (!path)
		return (NULL);
	if (check_path_access(path, mode))
		return (path);
	return (NULL);
}

char	*find_cmd_path(char *file, char **envp, char *env_var, int mode)
{
	int		indices[6];
	char	*result;

	indices[3] = 0;
	indices[1] = string_length(env_var, '\0');
	indices[0] = find_substr_index(envp, env_var, indices[1]);
	if (indices[0] < 0 || (file[0] && file[1] && file[0] == '.'
			&& file[1] == '/'))
	{
		result = check_path_without_env(file, mode);
		return (result);
	}
	indices[2] = string_length(envp[indices[0]], '\0');
	if (string_length(file, ' ') != string_length(file, '\0')
		&& !check_path_access(file, mode))
		return (NULL);
	while (envp[indices[0]][indices[1]])
	{
		result = check_env_path(envp[indices[0]], file, indices, mode);
		if (result)
			return (result);
		if (!indices[3])
			indices[3] = 1;
	}
	return (NULL);
}

char	*extract_next_substring(char *str, char del, int *index)
{
	char	*sub;
	int		size;
	int		a;

	while (str[index[0]] && str[index[0]] == del)
		index[0] += 1;
	size = string_length(str + index[0], del);
	sub = galloc(size + 1);
	if (!sub)
		return (NULL);
	a = 0;
	while (str[index[0]] && str[index[0]] != del)
	{
		if (str[index[0]] != 34 && str[index[0]] != 39)
			sub[a++] = str[index[0]];
		index[0] += 1;
	}
	sub[a] = '\0';
	index[0] += 1;
	return (sub);
}

int	count_tokens(char *str, char del)
{
	int	a;
	int	b;
	int	res;

	a = 0;
	b = 1;
	res = 0;
	while (str && str[a])
	{
		if (str[a] != del)
		{
			if (b)
				res += 1;
			b = 0;
		}
		else
			b = 1;
		a += 1;
	}
	return (res);
}
