/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exec5.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amsagman <amsagman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 12:19:59 by amsagman          #+#    #+#             */
/*   Updated: 2025/08/24 12:20:02 by amsagman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	handle_sign(char *str, size_t i, int *has_sign)
{
	if (str[i] == '+' || str[i] == '-')
	{
		*has_sign = 1;
		i++;
	}
	return (i);
}

static size_t	count_digits(char *str)
{
	size_t	digit_count;
	size_t	temp_i;

	digit_count = 0;
	temp_i = 0;
	while (str[temp_i] && ft_isdigit(str[temp_i]))
	{
		digit_count++;
		temp_i++;
	}
	return (digit_count);
}

static int	validate_digits(char *str, size_t *i)
{
	while (str[*i])
	{
		if (str[*i] == ' ' || str[*i] == '\t')
		{
			while (str[*i] == ' ' || str[*i] == '\t')
				(*i)++;
			break ;
		}
		if (!ft_isdigit(str[*i]))
			return (0);
		(*i)++;
	}
	return (1);
}

static int	check_trailing_whitespace(char *str, size_t i)
{
	while (str[i])
	{
		if (str[i] != ' ' && str[i] != '\t')
			return (0);
		i++;
	}
	return (1);
}

int	is_numeric_string(char *str)
{
	size_t	i;
	int		has_sign;
	size_t	digit_count;

	i = 0;
	has_sign = 0;
	if (!str || !*str)
		return (0);
	while (str[i] == ' ' || str[i] == '\t')
		i++;
	i = handle_sign(str, i, &has_sign);
	if (!str[i])
		return (0);
	digit_count = count_digits(&str[i]);
	if (digit_count > 19)
		return (0);
	if (!validate_digits(str, &i))
		return (0);
	return (check_trailing_whitespace(str, i));
}
