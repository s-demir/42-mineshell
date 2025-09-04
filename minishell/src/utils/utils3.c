/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amsagman <amsagman@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 17:51:11 by sedemir           #+#    #+#             */
/*   Updated: 2025/08/12 23:49:16 by amsagman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	strtoint(char *str)
{
	int	num;
	int	a;

	num = 0;
	a = 0;
	while (str[a] == ' ' || str[a] == '\t')
		a++;
	if (str[a] == '-')
		return (255);
	if (str[a] == '+')
		a++;
	while (str[a] >= '0' && str[a] <= '9')
	{
		num = (num * 10) + (str[a] - '0');
		a++;
	}
	return (num);
}

int	count_array_strings(char **arr)
{
	int	a;

	a = 0;
	while (arr[a] != 0)
		a++;
	return (a);
}

int	count_in_digits(int num)
{
	int	a;

	a = 0;
	if (!num)
		return (1);
	while (num)
	{
		num /= 10;
		a++;
	}
	return (a);
}

char	*build_pwd_string(char *new_path)
{
	char	*pwd_str;
	int		len;
	int		idx;

	len = string_length(new_path, '\0') + 5;
	pwd_str = galloc(len * sizeof(char));
	if (!pwd_str)
		return (NULL);
	duplicate_string(pwd_str, "PWD=", 0, 4);
	idx = 0;
	while (new_path[idx])
	{
		pwd_str[4 + idx] = new_path[idx];
		idx++;
	}
	pwd_str[4 + idx] = '\0';
	return (pwd_str);
}

t_env	*get_env(void)
{
	static t_env	env;

	return (&env);
}
