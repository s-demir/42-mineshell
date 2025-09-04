/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expand3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amsagman <amsagman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 12:19:19 by amsagman          #+#    #+#             */
/*   Updated: 2025/08/24 12:19:22 by amsagman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	find_var_end(char *var, int start)
{
	int	end;

	end = start + 1;
	if (var[end] == '?')
		end++;
	else
	{
		while (var[end] && is_valid_var_start(var, end, 0))
			end++;
	}
	return (end);
}

static char	*get_var_value(t_env *env, char *var_name)
{
	int	index;

	if (str_cmp(var_name, "_", NULL))
	{
		index = locate_env_var_index(env, "_");
		if (index >= 0)
			return (env->parsed_env[index][1]);
		return ("");
	}
	index = locate_env_var_index(env, var_name);
	if (index >= 0)
		return (env->parsed_env[index][1]);
	return ("");
}

char	*expand_variable_in_string(char *var, t_env *env, int *f_hole)
{
	int		start;
	int		end;
	char	*var_name;
	char	*value;

	start = *f_hole;
	end = find_var_end(var, start);
	var_name = galloc(end - start);
	duplicate_string(var_name, var, start + 1, end);
	if (str_cmp(var_name, "?", NULL) && g_var_thing)
		set_env_status(env, g_var_thing, "?=");
	g_var_thing = 0;
	value = get_var_value(env, var_name);
	*f_hole = string_length(value, '\0') + start;
	return (replace_var_with_value(var, value, start, end));
}
