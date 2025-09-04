/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amsagman <amsagman@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 16:47:38 by sedemir           #+#    #+#             */
/*   Updated: 2025/08/14 00:32:58 by amsagman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	copy_path_components(char *tmp_path, char *env_var, char *file,
		int *indx_s)
{
	int	b;
	int	file_idx;
	int	file_size;

	file_size = indx_s[5];
	b = 0;
	while (b < ((indx_s[1] - indx_s[4]) + file_size + 1))
	{
		if (indx_s[3] && (b < indx_s[1] - indx_s[4]))
			tmp_path[b] = env_var[indx_s[4] + b];
		else if (indx_s[3] && env_var[indx_s[1] - 1] != '/' && (b == indx_s[1]
				- indx_s[4]))
			tmp_path[b] = '/';
		else
		{
			file_idx = b - (indx_s[1] - indx_s[4]) - indx_s[3];
			if (file_idx >= 0 && file_idx < file_size)
				tmp_path[b] = file[file_idx];
			else
				tmp_path[b] = '\0';
		}
		b++;
	}
	tmp_path[b] = '\0';
}

char	*build_path_from_env(char *env_var, char *file, int *indx_s)
{
	char	*tmp_path;
	int		a;
	int		file_size;

	advance_path_index(env_var, indx_s, &a);
	file_size = string_length(file, ' ');
	indx_s[4] = a;
	indx_s[5] = file_size;
	tmp_path = galloc((indx_s[1] - a) + file_size + 2);
	if (!tmp_path)
		return (NULL);
	copy_path_components(tmp_path, env_var, file, indx_s);
	return (tmp_path);
}

int	find_substr_index(char **haystack, char *needle, int n_length)
{
	int	a;
	int	b;

	a = 0;
	while (haystack[a] != 0)
	{
		if (haystack[a][0] == needle[0])
		{
			b = 0;
			while (haystack[a][b] && haystack[a][b] == needle[b])
			{
				if (b == n_length - 1)
					return (a);
				b++;
			}
		}
		a++;
	}
	return (-1);
}

int	check_path_access(char *path, int mode)
{
	if (access(path, mode) < 0)
		return (0);
	return (1);
}

void	advance_path_index(char *env_var, int *indx_s, int *a)
{
	if (indx_s[3])
	{
		indx_s[1] += 1;
		*a = indx_s[1];
		while (env_var[indx_s[1]] != '\0' && env_var[indx_s[1]] != ':')
			indx_s[1] += 1;
	}
	else
		*a = indx_s[1];
}
