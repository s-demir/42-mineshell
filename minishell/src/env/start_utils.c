/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sedemir <sedemir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 15:12:52 by amsagman          #+#    #+#             */
/*   Updated: 2025/08/06 18:10:57 by sedemir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	fill_env_copy(char **env, char **new_env, int count)
{
	int	idx;

	idx = 0;
	while (idx < count)
	{
		new_env[idx] = strcopy(env[idx]);
		idx++;
	}
	new_env[idx] = NULL;
}

static char	**clone_env(char **env)
{
	char	**new_env;
	int		count;

	count = 0;
	if (!env)
		return (NULL);
	while (env[count])
		count++;
	new_env = galloc((count + 1) * sizeof(char *));
	if (!new_env)
		return (NULL);
	fill_env_copy(env, new_env, count);
	return (new_env);
}

static void	fill_env_pair(t_env *env, char **orig_env, int idx)
{
	int	len;

	len = string_length(orig_env[idx], '=');
	env->parsed_env[idx] = galloc(2 * sizeof(char *));
	if (!env->parsed_env[idx])
		return ;
	env->parsed_env[idx][0] = galloc((len + 1) * sizeof(char));
	env->parsed_env[idx][1] = galloc((string_length(orig_env[idx], '\0') - len
				+ 1) * sizeof(char));
	if (!env->parsed_env[idx][0] || !env->parsed_env[idx][1])
		return ;
	duplicate_string(env->parsed_env[idx][0], orig_env[idx], 0, len);
	duplicate_string(env->parsed_env[idx][1], orig_env[idx], len + 1,
		string_length(orig_env[idx], '\0'));
}

int	setup_env(t_env *env, char **orig_env, int count)
{
	int	idx;

	env->original_env = clone_env(orig_env);
	if (!env->original_env)
		return (0);
	while (orig_env[count])
		count++;
	env->parsed_env = galloc((count + 1) * sizeof(char **));
	if (!env->parsed_env)
	{
		return (0);
	}
	idx = 0;
	while (idx < count)
	{
		fill_env_pair(env, orig_env, idx);
		idx++;
	}
	env->parsed_env[idx] = NULL;
	return (1);
}
