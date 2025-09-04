/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amsagman <amsagman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 16:23:38 by sedemir           #+#    #+#             */
/*   Updated: 2025/08/24 05:38:13 by amsagman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	setup_child_fds(int *fd, int *out_fd, int *piped, int *child_fd)
{
	if (piped[PIPE_ACTIVE] && piped[PIPE_ACTIVE] <= piped[PIPE_TOTAL])
		dup2(child_fd[0], 0);
	if (piped[PIPE_ACTIVE] > 1)
		dup2(fd[1], 1);
	else
		safe_close(child_fd[0]);
	close_pipe_fds(fd[0], fd[1]);
	dup2(1, out_fd[1]);
}

void	manage_redir_fds(int *out_fd, int *piped)
{
	out_fd[1] = 1;
	if (piped[PIPE_REDIR_COMBO] && piped[PIPE_HAS_OUTPUT])
		out_fd[1] = piped[PIPE_OUTPUT_FD];
	if (piped[PIPE_ACTIVE] > 1 && (!piped[PIPE_REDIR_COMBO]
			|| !piped[PIPE_HAS_OUTPUT]))
		pipe(out_fd);
}

static void	execute_builtin_child_process(int *fd, int *out_fd,
				int *piped)
{
	if (fd[0] != -1)
		dup2(fd[0], 0);
	if (piped[PIPE_ACTIVE] > 1 && (!piped[PIPE_REDIR_COMBO]
			|| !piped[PIPE_HAS_OUTPUT]))
		dup2(out_fd[1], 1);
	safe_close(out_fd[1]);
	clear_garbage();
}

int	run_redir_builtin(char **cmd, int *fd, t_env *env, int *piped)
{
	pid_t	pid;
	int		out_fd[2];
	int		status;

	if (piped[PIPE_ACTIVE] > 1 && (!piped[PIPE_REDIR_COMBO]
			|| !piped[PIPE_HAS_OUTPUT]))
	{
		ft_memset(out_fd, -1, sizeof(out_fd));
		if (pipe(out_fd) == -1)
		{
			perror("pipe");
			return (1);
		}
		manage_redir_fds(out_fd, piped);
	}
	pid = fork();
	if (!pid)
	{
		execute_builtin_child_process(fd, out_fd, piped);
		exit(execute_builtin(cmd, fd, env, piped));
	}
	safe_close(out_fd[1]);
	waitpid(pid, &status, 0);
	return (get_exit_status(status));
}
