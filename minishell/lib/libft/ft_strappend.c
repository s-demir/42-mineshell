/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strappend.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sedemir <sedemir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 17:35:42 by amsagman          #+#    #+#             */
/*   Updated: 2025/08/06 18:10:57 by sedemir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_strappend(char *s1, char *s2, size_t len)
{
	char	*s3;
	int		i;
	int		j;

	j = len;
	i = ft_strlen(s1);
	s3 = (char *)galloc(i + j + 1);
	if (s3 == NULL)
		return (gfree(s1), NULL);
	ft_memcpy(s3, s1, i);
	ft_memcpy(s3 + i, s2, j);
	s3[i + j] = '\0';
	return (s3);
}
