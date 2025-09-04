/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sedemir <sedemir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 16:17:11 by sedemir           #+#    #+#             */
/*   Updated: 2025/08/06 18:10:57 by sedemir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	setup_args(t_ast_node *node, t_token **tokens)
{
	int	count;

	count = count_arguments(*tokens);
	node->args = galloc(sizeof(char *) * (count + 1));
	if (!node->args)
		return ;
	set_args(node, tokens, count);
}

t_ast_node	*build_command(t_token **tokens)
{
	t_ast_node	*node;

	node = make_node(TOKEN_WORD);
	if (!node)
		return (NULL);
	setup_args(node, tokens);
	if (!node->args)
		return (NULL);
	return (node);
}

void	init_file(t_ast_node *node, t_token *token)
{
	node->args = galloc(sizeof(char *) * 2);
	if (!node->args)
		return ;
	node->args[0] = token->value;
	node->args[1] = NULL;
	node->type = token->type;
	node->left = NULL;
	node->right = NULL;
}
