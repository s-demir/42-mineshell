/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sedemir <sedemir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 14:33:18 by amsagman          #+#    #+#             */
/*   Updated: 2025/08/06 18:10:57 by sedemir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static int	ft_count_words(char const *s, char sep)
{
	size_t	count;

	if (!*s || !s)
		return (0);
	count = 0;
	while (*s)
	{
		while (*s == sep)
			s++;
		if (*s)
		{
			count++;
			while (*s && *s != sep)
				s++;
		}
	}
	return (count);
}

static char	**ft_split_words(char **res, char const *s, char sep, int wordcnt)
{
	char const	*start;
	int			i;

	i = 0;
	while (*s && i < wordcnt)
	{
		while (*s == sep)
			s++;
		start = s;
		while (*s && *s != sep)
			s++;
		res[i] = ft_substr(start, 0, s - start);
		if (!res[i])
		{
			while (i > 0)
				gfree(res[--i]);
			gfree(res);
			return (NULL);
		}
		i++;
	}
	res[i] = NULL;
	return (res);
}

char	**ft_split(char const *s, char c)
{
	int		wordcount;
	char	**result;

	if (!s)
		return (NULL);
	wordcount = ft_count_words(s, c);
	result = (char **)galloc(sizeof(char *) * (wordcount + 1));
	if (!result)
		return (NULL);
	return (ft_split_words(result, s, c, wordcount));
}
