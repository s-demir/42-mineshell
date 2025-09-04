/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_handler3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sedemir <sedemir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 17:14:02 by sedemir           #+#    #+#             */
/*   Updated: 2025/08/06 18:26:44 by sedemir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_environment_var(char *var, t_env *env)
{
	int		c;
	int		o;
	char	*env_var;

	c = string_length(var, '=');
	env_var = galloc(c + 1);
	if (!env_var)
		return ;
	duplicate_string(env_var, var, 0, c);
	o = locate_env_var_index(env, env_var);
	if (o >= 0)
		remove_env_entry(env, o);
	if (c > 0 && c < string_length(var, '\0') - 1)
		add_env_entry(env, var, c, 1);
	else if (var[c] == '=')
		add_env_entry(env, var, c, 0);
	else if (c == string_length(var, '\0'))
		add_env_entry(env, var, c, -1);
}

void	set_env_status(t_env *env, int status, char *start)
{
	char	*var;
	int		a;

	a = count_in_digits(status) + string_length(start, '\0') + 1;
	var = galloc(a);
	if (!var)
		return ;
	duplicate_string(var, start, 0, string_length(start, '\0'));
	var[--a] = '\0';
	if (!status)
		var[--a] = '0';
	while (status)
	{
		var[--a] = (status % 10) + 48;
		status /= 10;
	}
	update_environment_var(var, env);
}

int	is_space(char *line)
{
	int	a;

	a = 0;
	while (line[a] == ' ' || line[a] == '\t' || line[a] == '\n')
		a += 1;
	if (line[a] == '\0')
		return (1);
	return (0);
}

char	*str_without_char(char *str, char del)
{
	int		a;
	int		b;
	char	*new_str;

	a = 0;
	b = 0;
	new_str = galloc(string_length(str, '\0') + 1);
	if (!new_str)
		return (NULL);
	while (str[a])
	{
		if (str[a] != del)
			new_str[b++] = str[a];
		a++;
	}
	new_str[b] = '\0';
	return (new_str);
}
