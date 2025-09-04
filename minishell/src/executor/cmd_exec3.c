/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exec3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amsagman <amsagman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 12:19:32 by amsagman          #+#    #+#             */
/*   Updated: 2025/08/24 12:19:36 by amsagman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_commands(t_ast_node *head, t_env *env, int *status)
{
	int	_piped[PIPE_STATE_SIZE];
	int	_status;

	if (!head || !env || !status)
		return ;
	env->piped = _piped;
	initialize_or_reset_pipe_state(_piped, 1);
	count_redirects(head, _piped);
	initialize_or_reset_pipe_state(_piped, 0);
	set_node_types(head);
	expand_variables_in_ast(head, env);
	_status = check_cmd_perms(head, env->original_env);
	if (!_status)
		*status = execute_ast_node(head, _piped, env);
	else
		*status = _status;
	set_env_status(env, *status, "?=");
}

static int	handle_input_redirection(t_ast_node *head, int *_piped, t_env *env,
		int status)
{
	if (head->file_type == READ_FILE)
	{
		switch_fds_identifier(_piped, PIPE_HAS_INPUT, PIPE_INPUT_FD, 1);
		_piped[PIPE_INPUT_FD] = open(head->args[0], O_RDONLY);
		if (_piped[PIPE_INPUT_FD] < 0)
		{
			perror(head->args[0]);
			status = switch_fds_identifier(_piped, 0, 0, 0);
		}
	}
	else if (head->file_type == READ_FROM_APPEND)
	{
		switch_fds_identifier(_piped, PIPE_HAS_INPUT, PIPE_INPUT_FD, 1);
		status = run_heredoc(head->args[0], _piped, env);
		signal(SIGINT, handle_ctrl_c);
	}
	return (status);
}

int	open_file_for_redirection(t_ast_node *head, int *_piped, t_env *env,
		int status)
{
	int	mode;

	if (!head || !_piped || !head->args || !head->args[0])
		return (1);
	if (head->file_type == READ_FILE || head->file_type == READ_FROM_APPEND)
	{
		return (handle_input_redirection(head, _piped, env, status));
	}
	else
	{
		switch_fds_identifier(_piped, PIPE_HAS_OUTPUT, PIPE_OUTPUT_FD, 1);
		mode = O_TRUNC;
		if (head->file_type == WRITE_FILE_APPEND)
			mode = O_APPEND;
		_piped[PIPE_OUTPUT_FD] = open(head->args[0], O_WRONLY | O_CREAT | mode,
				0666);
		if (_piped[PIPE_OUTPUT_FD] < 0)
		{
			perror(head->args[0]);
			return (1);
		}
	}
	return (status);
}
