/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exec_init.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amsagman <amsagman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 16:54:24 by sedemir           #+#    #+#             */
/*   Updated: 2025/08/02 17:55:58 by amsagman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	set_special_type(t_ast_node *head)
{
	if (head->type == TOKEN_REDIR_OUT && head->right)
		head->right->file_type = WRITE_FILE;
	if (head->type == TOKEN_REDIR_APPEND && head->right)
		head->right->file_type = WRITE_FILE_APPEND;
	if (head->type == TOKEN_REDIR_IN && head->right)
		head->right->file_type = READ_FILE;
	if (head->type == TOKEN_REDIR_HEREDOC && head->right)
		head->right->file_type = READ_FROM_APPEND;
	if (head->type == TOKEN_PIPE)
	{
		if (head->right)
			head->right->file_type = EXECUTE_FILE;
		if (head->left)
			head->left->file_type = EXECUTE_FILE;
	}
}

void	set_node_types(t_ast_node *head)
{
	if (head->type != TOKEN_WORD)
	{
		head->file_type = FILE_READY;
		set_special_type(head);
	}
	if (head->file_type == FILE_TYPE_NONE)
		head->file_type = EXECUTE_FILE;
	if (head->left)
		set_node_types(head->left);
	if (head->right)
		set_node_types(head->right);
}

static void	update_counts(t_ast_node *head, int *piped)
{
	if (head->type == TOKEN_REDIR_OUT || head->type == TOKEN_REDIR_APPEND)
		piped[PIPE_OUTPUT_COUNT]++;
	if (head->type == TOKEN_REDIR_IN || head->type == TOKEN_REDIR_HEREDOC)
		piped[PIPE_INPUT_COUNT]++;
	if (head->type == TOKEN_PIPE)
		piped[PIPE_TOTAL]++;
}

void	count_redirects(t_ast_node *head, int *piped)
{
	head->file_type = FILE_TYPE_NONE;
	update_counts(head, piped);
	if (head->left)
		count_redirects(head->left, piped);
	if (head->right)
		count_redirects(head->right, piped);
}
