/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sedemir <sedemir@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 16:21:06 by sedemir           #+#    #+#             */
/*   Updated: 2025/09/04 17:52:46 by sedemir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	setup_redirect(t_ast_node *node, t_token **tokens, t_token *tmp)
{
	if (!node || !tmp || !tmp->next)
		return ;
	node->left = parse_redirection(tokens);
	node->right = make_file(tmp->next);
}

t_ast_node	*link_redirect(t_token **tokens, t_token *tmp)
{
	t_ast_node		*node;
	t_token_type	type;

	type = (*tokens)->type;
	node = make_node(type);
	if (!node)
		return (NULL);
	if (!tmp || !tmp->next)
		return (NULL);
	*tokens = (*tokens)->next->next;
	setup_redirect(node, tokens, tmp);
	return (node);
}

int	count_arguments(t_token *current)
{
	int	count;

	count = 0;
	if (!current || current->type != TOKEN_WORD)
		return (count);
	count = 1;
	count += count_arguments(current->next);
	return (count);
}

void	set_args(t_ast_node *command_node, t_token **tokens, int arg_count)
{
	int	i;

	i = 0;
	while (i < arg_count)
	{
		command_node->args[i] = ft_strdup((*tokens)->value);
		*tokens = (*tokens)->next;
		i++;
	}
	command_node->args[arg_count] = NULL;
}
