/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sedemir <sedemir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 17:09:33 by sedemir           #+#    #+#             */
/*   Updated: 2025/08/06 18:10:57 by sedemir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_var_start(char *str, int index, int con)
{
	if ((con && str[index] == '$' && str[index + 1] && str[index + 1] != '$'
			&& !ft_isspace(str[index + 1]) && (ft_isalnum(str[index + 1])
				|| str[index + 1] == '_' || str[index + 1] == '?')) || (!con
			&& str[index] && str[index] != '$' && !ft_isspace(str[index])
			&& (ft_isalnum(str[index]) || str[index] == '_'
				|| str[index] == '?')))
		return (1);
	return (0);
}

void	append_env_var(char *var, t_env *env)
{
	int		b;
	int		c;
	int		d;
	int		o;
	char	*env_var;

	c = string_length(var, '+');
	d = string_length(var, '\0') - c - 1;
	env_var = galloc(c + 1);
	duplicate_string(env_var, var, 0, c);
	o = locate_env_var_index(env, env_var);
	if (o >= 0)
	{
		b = string_length(env->original_env[o], '\0');
		env_var = galloc(b + d + 1);
		if (!env_var)
			return ;
		duplicate_string(env_var, env->original_env[o], 0, b);
		duplicate_string(env_var + b, var, c + 2, string_length(var, '\0'));
	}
	else
		env_var = str_without_char(var, '+');
	update_environment_var(env_var, env);
}

int	locate_env_var_index(t_env *env, char *name)
{
	int	a;

	a = 0;
	while (env->parsed_env[a] != 0)
	{
		if (str_cmp(env->parsed_env[a][0], name, NULL))
			return (a);
		a++;
	}
	return (-1);
}
