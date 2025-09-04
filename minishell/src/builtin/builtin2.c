/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amsagman <amsagman@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 17:03:26 by sedemir           #+#    #+#             */
/*   Updated: 2025/08/14 22:53:38 by amsagman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_env_var_to_fd(char *str_1, char *str_2, int fd)
{
	if (!str_cmp(str_1, "?", NULL) && str_2[0])
	{
		ft_putstr_fd(str_1, fd);
		if (str_2)
		{
			ft_putstr_fd("=", fd);
		}
		if (str_2[0])
			ft_putstr_fd(str_2, fd);
		write(fd, "\n", 1);
	}
}

void	print_export_vars(char ***arr, int a, int fd)
{
	while (a--)
	{
		if (!str_cmp(arr[a][0], "?", NULL))
		{
			ft_putstr_fd("declare -x ", fd);
			ft_putstr_fd(arr[a][0], fd);
			if (arr[a][1] && arr[a][1][0] != '\0')
			{
				write(fd, "=", 1);
				ft_putchar_fd('"', fd);
				ft_putstr_fd(arr[a][1], fd);
				ft_putchar_fd('"', fd);
			}
			write(fd, "\n", 1);
		}
	}
}

int	compare_string_order(char *s1, char *s2)
{
	int	a;

	a = 0;
	while (s1[a] && s2[a])
	{
		if (s1[a] > s2[a])
			return (0);
		else if (s2[a] > s1[a])
			return (1);
		a++;
	}
	if (s2[a])
		return (1);
	else if (s1[a])
		return (0);
	return (0);
}

int	is_array_sorted(char ***array, int size)
{
	int	a;
	int	b;
	int	c;

	a = 0;
	while (a < size - 1)
	{
		b = a + 1;
		while (b < size)
		{
			c = compare_string_order(array[a][0], array[b][0]);
			if (c || (c < 0 && (b - a) > 1))
				return (0);
			b++;
		}
		a++;
	}
	return (1);
}

int	is_valid_echo_option(char *s)
{
	int	a;

	a = 0;
	if (s && s[a] && s[a] == '-')
	{
		while (s[++a])
		{
			if (s[a] != 'n')
				return (0);
		}
	}
	else
		return (0);
	return (1);
}
