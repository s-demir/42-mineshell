/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exec4.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amsagman <amsagman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 12:19:42 by amsagman          #+#    #+#             */
/*   Updated: 2025/08/24 12:19:45 by amsagman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	setup_pipe_if_needed(int *pipe_fd, int *piped)
{
	if (piped[PIPE_ACTIVE] > 1)
	{
		if (pipe(pipe_fd) == -1)
		{
			perror("pipe");
			return (1);
		}
	}
	return (0);
}

void	cleanup_parent_fds(int *pipe_fd, int *fd, int *piped)
{
	if (pipe_fd[1] > 2)
		safe_close(pipe_fd[1]);
	if (piped[PIPE_ACTIVE] > 1)
		fd[0] = pipe_fd[0];
	else
		safe_close(pipe_fd[0]);
}

static void	handle_basic_fds(int *piped, int *fd, int *pipe_fd)
{
	if (piped[PIPE_ACTIVE] && piped[PIPE_ACTIVE] <= piped[PIPE_TOTAL])
		dup2(fd[0], 0);
	if (piped[PIPE_ACTIVE] > 1)
		dup2(pipe_fd[1], 1);
	close_pipe_fds(pipe_fd[0], pipe_fd[1]);
}

int	run_basic_cmd(char **cmd, int *fd, char **env, int *piped)
{
	pid_t	pid;
	int		pipe_fd[2];

	ft_memset(pipe_fd, -1, sizeof(pipe_fd));
	if (setup_pipe_if_needed(pipe_fd, piped))
		return (1);
	pid = fork();
	signal(SIGINT, child_ctrl_c);
	signal(SIGQUIT, child_ctrl_c);
	if (pid == -1)
		return (perror("fork"), 1);
	if (!pid)
	{
		if (pipe_fd[0] > 2)
			safe_close(pipe_fd[0]);
		handle_basic_fds(piped, fd, pipe_fd);
		execve(cmd[0], cmd, env);
		ft_putstr_fd(cmd[0], 2);
		ft_putendl_fd(": command not found", 2);
		clear_garbage();
		exit(127);
	}
	cleanup_parent_fds(pipe_fd, fd, piped);
	return (1);
}
