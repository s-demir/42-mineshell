/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sedemir <sedemir@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 16:15:11 by sedemir           #+#    #+#             */
/*   Updated: 2025/09/04 17:52:46 by sedemir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast_node	*make_file(t_token *token)
{
	t_ast_node	*node;

	if (!token)
		return (NULL);
	node = galloc(sizeof(t_ast_node));
	if (!node)
		return (NULL);
	init_file(node, token);
	if (!node->args)
	{
		return (NULL);
	}
	return (node);
}

t_ast_node	*handle_redir_loop(t_token **tokens, t_token *tmp)
{
	t_token		*next;
	t_ast_node	*node;

	while (*tokens && (*tokens)->next)
	{
		next = (*tokens)->next;
		if (next->type >= TOKEN_REDIR_IN && next->type <= TOKEN_REDIR_HEREDOC)
		{
			if (!next->next)
				return (NULL);
			node = make_node(next->type);
			if (!node)
				return (NULL);
			(*tokens)->next = next->next->next;
			node->left = parse_redirection(&tmp);
			node->right = make_file(next->next);
			return (node);
		}
		*tokens = next;
	}
	return (build_command(&tmp));
}

t_ast_node	*parse_redirection(t_token **tokens)
{
	t_token	*tmp;

	tmp = *tokens;
	if (!tmp)
		return (NULL);
	if (tmp->type >= TOKEN_REDIR_IN && tmp->type <= TOKEN_REDIR_HEREDOC)
		return (link_redirect(tokens, tmp));
	return (handle_redir_loop(tokens, tmp));
}

t_ast_node	*par_pipe(t_token **tokens)
{
	t_token		*tmp;
	t_token		*next_token;
	t_ast_node	*pipe_node;

	tmp = *tokens;
	while (*tokens && (*tokens)->next)
	{
		next_token = (*tokens)->next;
		if ((*tokens)->next->type == TOKEN_PIPE)
		{
			pipe_node = make_node((*tokens)->next->type);
			(*tokens)->next = NULL;
			pipe_node->left = parse_redirection(&tmp);
			pipe_node->right = par_pipe(&(next_token->next));
			return (pipe_node);
		}
		*tokens = next_token;
	}
	return (parse_redirection(&tmp));
}

t_ast_node	*parse_tokens(t_token **tokens)
{
	if (!tokens || !*tokens)
		return (NULL);
	return (par_pipe(tokens));
}
