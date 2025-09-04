/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sedemir <sedemir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 16:13:17 by amsagman          #+#    #+#             */
/*   Updated: 2025/08/06 18:10:14 by sedemir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_strndup(const char *src, size_t n)
{
	size_t	i;
	size_t	size;
	char	*str;

	i = 0;
	size = ft_strnlen(src, n);
	str = galloc(size + 1);
	if (str == NULL)
		return (NULL);
	while (i < size)
	{
		str[i] = src[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}
