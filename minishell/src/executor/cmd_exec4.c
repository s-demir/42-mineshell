/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exec4.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amsagman <amsagman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 12:19:52 by amsagman          #+#    #+#             */
/*   Updated: 2025/08/24 12:19:53 by amsagman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	create_pipe_fds(int *fd_)
{
	fd_[0] = -1;
	fd_[1] = -1;
	if (pipe(fd_) == -1)
	{
		perror("pipe");
		return (0);
	}
	return (1);
}

static pid_t	fork_process(int *fd_)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		safe_close(fd_[0]);
		safe_close(fd_[1]);
		perror("fork");
		return (-1);
	}
	return (pid);
}

static void	handle_parent_process(int *_piped, int *_fd, int *fd_)
{
	parent_fds_managment(_piped, _fd, fd_);
	safe_close(fd_[0]);
	safe_close(fd_[1]);
}

int	run_redir_cmd(char **_cmd_, int *_fd, char **env, int *_piped)
{
	pid_t	pid;
	int		fd_[2];

	if (!_fd)
		return (1);
	if (!create_pipe_fds(fd_))
		return (1);
	pid = fork_process(fd_);
	if (pid == -1)
		return (1);
	signal(SIGINT, child_ctrl_c);
	signal(SIGQUIT, child_ctrl_c);
	if (!pid)
	{
		child_fds_managment(_piped, _fd, fd_);
		execve(_cmd_[0], _cmd_, env);
		perror("execve failed");
		clear_garbage();
		exit(127);
	}
	handle_parent_process(_piped, _fd, fd_);
	return (1);
}
