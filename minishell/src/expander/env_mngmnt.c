/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_mngmnt.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amsagman <amsagman@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 17:12:17 by sedemir           #+#    #+#             */
/*   Updated: 2025/08/14 22:53:38 by amsagman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**copy_env_exclude_index(char **env, int a, int a_2, int d)
{
	char	**new_thing;
	int		b;
	int		c;

	b = 0;
	c = 0;
	new_thing = galloc((a + a_2) * sizeof(char **));
	if (!new_thing)
		return (NULL);
	while (b < a)
	{
		if (b != d)
			new_thing[c++] = strcopy(env[b]);
		b++;
	}
	if (d >= 0)
		new_thing[c] = 0;
	return (new_thing);
}

char	***duplicate_env_structure(t_env *env, int a, int a_2, int d)
{
	int		b;
	int		c;
	char	***new_thing;

	b = 0;
	c = 0;
	new_thing = galloc((a + a_2) * sizeof(char ***));
	while (b < a)
	{
		if (b != d)
		{
			new_thing[c] = galloc(2 * sizeof(char **));
			if (!new_thing[c])
				return (NULL);
			new_thing[c][0] = strcopy(env->parsed_env[b][0]);
			new_thing[c][1] = strcopy(env->parsed_env[b][1]);
			c++;
		}
		b++;
	}
	if (d >= 0)
		new_thing[c] = 0;
	return (new_thing);
}

void	remove_env_entry(t_env *env, int c)
{
	int	a;

	a = 0;
	while (env->parsed_env[a] != 0)
		a++;
	if (a > 0)
	{
		env->parsed_env = duplicate_env_structure(env, a, 0, c);
		env->original_env = copy_env_exclude_index(env->original_env, a, 0, c);
	}
}

static void	set_env_value_at_index(t_env *env, char *cmd, int a, int con)
{
	int	b;
	int	c;

	b = string_length(cmd, '=');
	c = string_length(cmd, '\0') - b;
	if (con > 0)
	{
		env->parsed_env[a][1] = galloc(c + 1);
		if (!env->parsed_env[a][1])
			return ;
		duplicate_string(env->parsed_env[a][1], cmd, b + 1, b + c);
	}
	else
	{
		env->parsed_env[a][1] = galloc(1);
		if (!env->parsed_env[a][1])
			return ;
		env->parsed_env[a][1][0] = '\0';
	}
}

void	add_env_entry(t_env *env, char *cmd, int b, int con)
{
	int	a;
	int	c;

	a = 0;
	c = 2;
	if (con)
		c = string_length(cmd, '\0') - b;
	while (env->parsed_env[a] != 0)
		a++;
	env->parsed_env = duplicate_env_structure(env, a, 2, -1);
	env->original_env = copy_env_exclude_index(env->original_env, a, 2, -1);
	env->original_env[a] = galloc(b + c + 1);
	env->parsed_env[a] = galloc(2 * sizeof(char *));
	env->parsed_env[a][0] = galloc(b + 1);
	if (!env->parsed_env[a] || !env->parsed_env[a][0] || !env->original_env[a])
		return ;
	duplicate_string(env->parsed_env[a][0], cmd, 0, b);
	if (con > 0)
		duplicate_string(env->original_env[a], cmd, 0, string_length(cmd,
				'\0'));
	else
		duplicate_string(env->original_env[a], "  ", 0, 1);
	set_env_value_at_index(env, cmd, a, con);
	env->original_env[a + 1] = 0;
	env->parsed_env[a + 1] = 0;
}
