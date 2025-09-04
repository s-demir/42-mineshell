/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amsagman <amsagman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 16:52:13 by sedemir           #+#    #+#             */
/*   Updated: 2025/08/17 02:09:57 by amsagman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	child_fds_managment(int *_piped, int *_fd, int *fd_)
{
	if (!_piped || !_fd || !fd_)
		return ;
	if (_piped[PIPE_REDIR_COMBO] && _piped[PIPE_HAS_INPUT])
	{
		dup2(_piped[PIPE_INPUT_FD], 0);
		safe_close(_piped[PIPE_INPUT_FD]);
	}
	if (_piped[PIPE_REDIR_COMBO] && _piped[PIPE_HAS_OUTPUT])
	{
		dup2(_piped[PIPE_OUTPUT_FD], 1);
		safe_close(_piped[PIPE_OUTPUT_FD]);
	}
	if (_piped[PIPE_ACTIVE] && _piped[PIPE_ACTIVE] <= _piped[PIPE_TOTAL]
		&& (!_piped[PIPE_REDIR_COMBO] || !_piped[PIPE_HAS_INPUT]))
	{
		if (_fd[0] >= 0)
			dup2(_fd[0], 0);
	}
	if (_piped[PIPE_ACTIVE] > 1 && (!_piped[PIPE_REDIR_COMBO]
			|| !_piped[PIPE_HAS_OUTPUT]))
	{
		if (fd_[1] >= 0)
			dup2(fd_[1], 1);
	}
}

void	parent_fds_managment(int *_piped, int *_fd, int *fd_)
{
	if (_piped[PIPE_REDIR_COMBO] && _piped[PIPE_HAS_INPUT])
	{
		safe_close(_piped[PIPE_INPUT_FD]);
		_piped[PIPE_HAS_INPUT] = 0;
	}
	if (_piped[PIPE_REDIR_COMBO] && _piped[PIPE_HAS_OUTPUT])
	{
		safe_close(_piped[PIPE_OUTPUT_FD]);
		_piped[PIPE_HAS_OUTPUT] = 0;
	}
	if (!_piped[PIPE_HAS_OUTPUT] && !_piped[PIPE_HAS_INPUT])
		_piped[PIPE_REDIR_COMBO] = 0;
	if (_piped[PIPE_ACTIVE] < _piped[PIPE_TOTAL])
	{
		_fd[0] = fd_[0];
	}
	else
	{
		safe_close(fd_[0]);
	}
}
