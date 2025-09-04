/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sedemir <sedemir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 17:24:20 by sedemir           #+#    #+#             */
/*   Updated: 2025/08/06 18:24:08 by sedemir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*remove_quotes_from_str(char *str, int si_q_c, int do_q_c, int a)
{
	char	*new_str;
	int		b;

	b = 0;
	new_str = galloc(string_length(str, '\0') + 1);
	while (str[a])
	{
		if (str[a] == 34 && !(si_q_c % 2))
			do_q_c++;
		else if (str[a] == 39 && !(do_q_c % 2))
			si_q_c++;
		if ((str[a] != 34 || si_q_c % 2) && ((str[a] != 39) || do_q_c % 2))
			new_str[b++] = str[a];
		a++;
	}
	new_str[b] = '\0';
	return (new_str);
}

int	str_cmp(char *s_1, char *s_2, char *s_3)
{
	int	a;

	a = 0;
	while (s_1[a] && s_2[a] && s_1[a] == s_2[a])
		a += 1;
	if (a == string_length(s_1, '\0') && a == string_length(s_2, '\0'))
		return (1);
	if (s_3)
		return (str_cmp(s_1, s_3, NULL));
	return (0);
}

int	string_length(char *str, char end)
{
	int	a;

	a = 0;
	while (str && str[a] != '\0' && str[a] != end)
		a += 1;
	return (a);
}

char	*strcopy(char *src)
{
	int		a;
	int		b;
	char	*dest;

	b = 0;
	while (src[b])
		b += 1;
	dest = galloc(b + 1);
	if (!dest)
		return (NULL);
	a = 0;
	while (a < b)
	{
		dest[a] = src[a];
		a += 1;
	}
	dest[a] = '\0';
	return (dest);
}

void	duplicate_string(char *dest, const char *src, int start, int end)
{
	int	index;

	index = 0;
	while (start < end)
		dest[index++] = src[start++];
	dest[index] = '\0';
}
