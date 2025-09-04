/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sedemir <sedemir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 16:19:27 by sedemir           #+#    #+#             */
/*   Updated: 2025/08/12 16:38:12 by sedemir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_node(t_ast_node *node, t_token_type type)
{
	node->type = type;
	node->args = NULL;
	node->left = NULL;
	node->right = NULL;
}

t_ast_node	*make_node(t_token_type type)
{
	t_ast_node	*node;

	node = galloc(sizeof(t_ast_node));
	if (!node)
	{
		return (NULL);
	}
	init_node(node, type);
	return (node);
}
