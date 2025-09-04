/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sedemir <sedemir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 13:19:58 by amsagman          #+#    #+#             */
/*   Updated: 2025/08/06 18:10:14 by sedemir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	len;
	char	*scopy;

	if (!s1 || !set)
		return (NULL);
	while (*s1 && ft_strchr(set, *s1) != NULL)
		s1++;
	len = ft_strlen(s1);
	while (len && ft_strchr(set, s1[len - 1]) != NULL)
		len--;
	scopy = (char *)galloc(sizeof(char) * (len + 1));
	if (!scopy)
		return (NULL);
	ft_memcpy(scopy, s1, len);
	scopy[len] = '\0';
	return (scopy);
}
