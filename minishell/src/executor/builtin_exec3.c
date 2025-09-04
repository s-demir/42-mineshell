/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exec3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amsagman <amsagman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 16:29:39 by sedemir           #+#    #+#             */
/*   Updated: 2025/08/24 05:48:37 by amsagman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	cleanup_output_fds(int *_piped, int *_out_fd, int *_fd)
{
	if (_piped[PIPE_REDIR_COMBO] && _piped[PIPE_HAS_OUTPUT])
	{
		safe_close(_out_fd[1]);
		_piped[PIPE_HAS_OUTPUT] = 0;
	}
	if (!_piped[PIPE_HAS_INPUT] && !_piped[PIPE_HAS_OUTPUT])
		_piped[PIPE_REDIR_COMBO] = 0;
	if (_piped[PIPE_ACTIVE] > 1 && (!_piped[PIPE_REDIR_COMBO]
			|| !_piped[PIPE_HAS_OUTPUT]))
	{
		safe_close(_out_fd[1]);
		_fd[0] = _out_fd[0];
	}
}

int	execute_builtin_with_piping(char **_cmd_, int *_fd, t_env *env, int *_piped)
{
	int	status;
	int	_out_fd[2];

	_out_fd[1] = 1;
	if (_piped[PIPE_ACTIVE] > 1)
	{
		_out_fd[0] = -1;
		_out_fd[1] = -1;
		if (pipe(_out_fd) == -1)
		{
			perror("pipe");
			return (1);
		}
	}
	status = run_builtin_in_child(_cmd_, env, _out_fd);
	if (_piped[PIPE_ACTIVE] > 1)
	{
		safe_close(_out_fd[1]);
		_fd[0] = _out_fd[0];
	}
	return (status);
}

int	execute_builtin_with_simple_piping(char **_cmd_, int *_fd, t_env *env,
		int *_piped)
{
	int	status;
	int	_out_fd[2];

	_out_fd[1] = 1;
	if (_piped[PIPE_REDIR_COMBO] && _piped[PIPE_HAS_OUTPUT])
		_out_fd[1] = _piped[PIPE_OUTPUT_FD];
	if (_piped[PIPE_ACTIVE] > 1 && (!_piped[PIPE_REDIR_COMBO]
			|| !_piped[PIPE_HAS_OUTPUT]))
	{
		_out_fd[0] = -1;
		_out_fd[1] = -1;
		if (pipe(_out_fd) == -1)
		{
			perror("pipe");
			return (1);
		}
	}
	status = run_builtin_in_child(_cmd_, env, _out_fd);
	cleanup_output_fds(_piped, _out_fd, _fd);
	return (status);
}
