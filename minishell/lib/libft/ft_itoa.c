/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sedemir <sedemir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 13:40:57 by amsagman          #+#    #+#             */
/*   Updated: 2025/08/06 18:13:18 by sedemir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

static int	len(long nb)
{
	int	len;

	len = 0;
	if (nb < 0)
	{
		nb *= -1;
		len++;
	}
	if (nb == 0)
	{
		len++;
	}
	while (nb > 0)
	{
		nb /= 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	char	*str;
	long	nb;
	int		i;

	nb = n;
	i = len(nb);
	str = (char *)galloc((size_t)(i + 1));
	if (!str)
		return (NULL);
	str[i] = '\0';
	if (nb == 0)
	{
		str[0] = '0';
		return (str);
	}
	if (nb < 0)
		str[0] = '-';
	if (nb < 0)
		nb = -nb;
	while (nb > 0)
	{
		str[--i] = (char)(nb % 10 + '0');
		nb /= 10;
	}
	return (str);
}
