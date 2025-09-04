/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sedemir <sedemir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 18:21:41 by sedemir           #+#    #+#             */
/*   Updated: 2025/08/07 18:32:42 by sedemir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "stdbool.h"
#include <stdlib.h>

static bool	gfree_from_ll(void *address);

void	*galloc(size_t size)
{
	void	*address;

	address = addgarbage(malloc(size));
	return (address);
}

void	*addgarbage(void *address)
{
	t_list	**collector;
	t_list	*tmp;

	if (!address)
		return (0);
	collector = getgarbage();
	tmp = malloc(sizeof(t_list));
	if (!tmp)
		return (0);
	tmp->next = 0;
	tmp->prev = 0;
	tmp->content = address;
	if (*collector)
	{
		tmp->next = *collector;
		tmp->next->prev = tmp;
		*collector = tmp;
	}
	else
		*collector = tmp;
	return (tmp->content);
}

void	clear_garbage(void)
{
	t_list	**collector;
	t_list	*todel;
	t_list	*tmp;

	collector = getgarbage();
	todel = *collector;
	while (todel)
	{
		free(todel->content);
		tmp = todel->next;
		free(todel);
		todel = tmp;
	}
	*collector = 0;
}

void	gfree(void *address)
{
	if (address)
		free(address);
	else if (!address)
		return ;
	else if (!gfree_from_ll(address))
		free(address);
}

static bool	gfree_from_ll(void *address)
{
	t_list	*todel;

	todel = *getgarbage();
	while (todel && address)
	{
		if (todel->content == address)
		{
			if (todel->prev)
			{
				todel->prev->next = todel->next;
				todel->next->prev = todel->prev;
			}
			else
			{
				todel->next->prev = 0;
				*getgarbage() = todel->next;
			}
			free(todel->content);
			free(todel);
			return (true);
		}
		todel = todel->next;
	}
	return (false);
}
