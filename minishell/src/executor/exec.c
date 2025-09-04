/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amsagman <amsagman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 16:37:44 by sedemir           #+#    #+#             */
/*   Updated: 2025/08/24 05:46:20 by amsagman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			g_var_thing;

int	handle_piped_cmd(t_ast_node *head, int *_piped, t_env *env, int *_fd)
{
	int	status;

	status = 0;
	if (head->file_type == EXECUTE_FILE)
	{
		_piped[PIPE_REDIR_COMBO] = 0;
		status = prep_run_cmd(head->args, _fd, _piped, env);
	}
	if (head->type == TOKEN_REDIR_IN || head->type == TOKEN_REDIR_OUT
		|| head->type == TOKEN_REDIR_APPEND
		|| head->type == TOKEN_REDIR_HEREDOC)
		return (manage_redir(head, _piped, env, _fd));
	if (head->left)
		status = handle_piped_cmd(head->left, _piped, env, _fd);
	if (head->right)
		status = handle_piped_cmd(head->right, _piped, env, _fd);
	return (status);
}

static int	process_redir(t_ast_node *head, int *piped, t_env *env, int *fd)
{
	int	status;

	status = 0;
	status = open_file_for_redirection(head->right, piped, env, status);
	if ((status || !head->left) && piped[PIPE_ACTIVE] > 1)
		piped[PIPE_ACTIVE]--;
	if (head->left && head->left->file_type == EXECUTE_FILE
		&& piped[PIPE_EXEC_STATE] && !status)
	{
		piped[PIPE_REDIR_COMBO] = 1;
		status = prep_run_cmd(head->left->args, fd, piped, env);
	}
	return (status);
}

int	manage_redir(t_ast_node *head, int *piped, t_env *env, int *fd)
{
	int	status;

	status = 0;
	piped[PIPE_EXEC_STATE] = 1;
	if (head->right)
		status = process_redir(head, piped, env, fd);
	if (head->left && head->left->type == TOKEN_PIPE && piped[PIPE_EXEC_STATE])
		status = handle_piped_cmd(head->left, piped, env, fd);
	if (head->left && (head->left->type == TOKEN_REDIR_IN
			|| head->left->type == TOKEN_REDIR_OUT
			|| head->left->type == TOKEN_REDIR_APPEND
			|| head->left->type == TOKEN_REDIR_HEREDOC))
		status = manage_redir(head->left, piped, env, fd);
	return (status);
}

static int	process_node(t_ast_node *head, int *piped, t_env *env, int *fd)
{
	int	status;

	status = 0;
	if (head->type == TOKEN_PIPE)
		status = handle_piped_cmd(head, piped, env, fd);
	if (head->type == TOKEN_REDIR_IN || head->type == TOKEN_REDIR_OUT
		|| head->type == TOKEN_REDIR_APPEND
		|| head->type == TOKEN_REDIR_HEREDOC)
		status = manage_redir(head, piped, env, fd);
	return (status);
}

int	execute_ast_node(t_ast_node *head, int *piped, t_env *env)
{
	int	fd[2];
	int	status;

	status = 0;
	fd[0] = -1;
	fd[1] = -1;
	if (head->file_type == FILE_READY)
		status = process_node(head, piped, env, fd);
	else if (head->file_type == EXECUTE_FILE)
		status = prep_run_cmd(head->args, fd, piped, env);
	status = wait_children(status, piped);
	safe_close(piped[PIPE_INPUT_FD]);
	safe_close(piped[PIPE_OUTPUT_FD]);
	safe_close(fd[0]);
	safe_close(fd[1]);
	g_var_thing = 0;
	return (status);
}
