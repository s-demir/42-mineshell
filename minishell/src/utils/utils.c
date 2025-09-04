/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amsagman <amsagman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 17:21:34 by sedemir           #+#    #+#             */
/*   Updated: 2025/08/16 20:22:01 by amsagman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_exit_status(int err)
{
	if (err == 2)
		return (127);
	else if (err == 13)
		return (126);
	return (err);
}

void	cleanup_shell(t_env *env, int status)
{
	if (env)
	{
		clear_garbage();
	}
	exit(status);
}

void	safe_close(int fd)
{
	if (fd > 2)
		close(fd);
}

void	close_pipe_fds(int fd1, int fd2)
{
	if (!fd1 && !fd2)
		return ;
	if (fd1 > 2)
		safe_close(fd1);
	if (fd2 > 2)
		safe_close(fd2);
	fd1 = -1;
	fd2 = -1;
}

void	cleanup_redirection_fds(int *piped)
{
	if (piped[PIPE_HAS_INPUT] && piped[PIPE_INPUT_FD] != -1)
		safe_close(piped[PIPE_INPUT_FD]);
	if (piped[PIPE_HAS_OUTPUT] && piped[PIPE_OUTPUT_FD] != -1)
		safe_close(piped[PIPE_OUTPUT_FD]);
}
