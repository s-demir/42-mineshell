/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expand2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sedemir <sedemir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 17:16:35 by sedemir           #+#    #+#             */
/*   Updated: 2025/08/06 18:10:14 by sedemir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	simplified_refactor_thing(char **array, int index, char *str, int in)
{
	int	size;

	if (!str[in] || in > string_length(str, '\0'))
		return (index);
	while (str[in] && str[in] == ' ')
		in++;
	size = string_length(str + in, ' ');
	if (size > string_length(str + in, 34))
		size = string_length(str + in, 34);
	if (size > string_length(str + in, 39))
		size = string_length(str + in, 39);
	if (str[in] == 34)
		size = string_length(str + in + 1, 34) + 2;
	if (str[in] == 39)
		size = string_length(str + in + 1, 39) + 2;
	if ((in + size) > string_length(str, '\0'))
		return (index);
	array[index] = galloc(size + 1);
	if (!array[index])
		return (index);
	duplicate_string(array[index], str, in, in + size);
	return (simplified_refactor_thing(array, index + 1, str, in + size));
}

static int	count_special_chars(char *str, int s_q, int d_q)
{
	int	a;

	a = 0;
	(void)s_q;
	while (str[a])
	{
		if (str[a] == 34 || str[a] == 39 || str[a] == '$')
			d_q++;
		a++;
	}
	return (d_q);
}

int	is_flawed_str(char *str, int a, int b, int res)
{
	int	si_q;
	int	do_q;

	si_q = 0;
	do_q = 0;
	while (str[a])
	{
		if (str[a] == 34)
			do_q++;
		else if (str[a] == 39)
			si_q++;
		else if (!(si_q % 2) && !(do_q % 2))
		{
			if (str[a] == ' ')
			{
				if (b)
					res++;
				b = 0;
			}
			else
				b = 1;
		}
		a++;
	}
	return (res);
}

int	count_arg_flaws(char **array)
{
	int	a;
	int	res;

	res = 0;
	a = 0;
	while (array[a])
	{
		res += is_flawed_str(array[a], 0, 0, 0);
		res += count_special_chars(array[a], 0, 0);
		a++;
	}
	return (res);
}

char	**refactore_args_array(char **args, int *quick_norm_fix)
{
	int		a;
	int		b;
	int		c;
	char	**new_args;

	a = 0;
	b = 0;
	*quick_norm_fix = 0;
	c = count_array_strings(args);
	new_args = galloc((count_arg_flaws(args) + c + 1) * sizeof(char **));
	if (!new_args)
		return (NULL);
	while (args[a])
	{
		c = is_flawed_str(args[a], 0, 0, 0);
		if (c)
			b = simplified_refactor_thing(new_args, b, args[a], 0);
		else
			new_args[b++] = strcopy(args[a]);
		a++;
	}
	new_args[b] = 0;
	return (new_args);
}
