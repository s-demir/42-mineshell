/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exec2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sedemir <sedemir@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 16:27:08 by sedemir           #+#    #+#             */
/*   Updated: 2025/09/04 17:51:51 by sedemir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	run_builtin_child(char **cmd, int *fd, t_env *env, int *piped)
{
	pid_t	pid;
	int		pipe_fd[2];
	int		out_fd[2];
	int		status;

	ft_memset(pipe_fd, -1, sizeof(pipe_fd));
	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		return (1);
	}
	pid = fork();
	if (!pid)
	{
		setup_child_fds(pipe_fd, out_fd, piped, fd);
		status = run_builtin_in_child(cmd, env, out_fd);
		clear_garbage();
		exit(WEXITSTATUS(status));
	}
	close_pipe_fds(pipe_fd[1], fd[0]);
	if (piped[PIPE_ACTIVE] > 1)
		fd[0] = pipe_fd[0];
	else
		safe_close(pipe_fd[0]);
	return (1);
}

char	**combine_command_arguments(char **f_args, char **cmd)
{
	size_t	index;
	char	**new_args;

	index = 1;
	if (!f_args)
		return (NULL);
	while (cmd[index])
		index++;
	new_args = galloc((index + 1) * sizeof(char *));
	if (!new_args)
		return (NULL);
	index = 0;
	if (f_args[0])
		new_args[0] = strcopy(f_args[0]);
	else
		new_args[0] = strcopy("");
	index = 1;
	while (cmd[index])
	{
		new_args[index] = strcopy(cmd[index]);
		index++;
	}
	new_args[index] = NULL;
	return (new_args);
}

static void	handle_exit_error(char *arg, t_env *env)
{
	ft_putendl_fd("exit", STDOUT_FILENO);
	ft_putstr_fd("exit: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putendl_fd(": numeric argument required", 2);
	cleanup_shell(env, 2);
}

static int	handle_exit_with_args(char **args, t_env *env)
{
	int	exit_status;

	if (args[1] && args[2])
	{
		if (!is_numeric_string(args[1]))
			handle_exit_error(args[1], env);
		ft_putendl_fd("exit", STDOUT_FILENO);
		ft_putendl_fd("exit: too many arguments", 2);
		return (1);
	}
	else if (args[1])
	{
		if (!is_numeric_string(args[1]))
			handle_exit_error(args[1], env);
		exit_status = strtoint(args[1]);
		ft_putendl_fd("exit", STDOUT_FILENO);
		cleanup_shell(env, exit_status);
	}
	else
	{
		ft_putendl_fd("exit", STDOUT_FILENO);
		cleanup_shell(env, 0);
	}
	return (0);
}

int	execute_builtin(char **args, int *fd, t_env *env, int *piped)
{
	if (str_cmp(args[0], "exit", NULL))
		return (handle_exit_with_args(args, env));
	else if (!piped[PIPE_REDIR_COMBO])
		return (execute_builtin_with_piping(args, fd, env, piped));
	else
		return (execute_builtin_with_simple_piping(args, fd, env, piped));
	return (run_redir_builtin(args, fd, env, piped));
}
