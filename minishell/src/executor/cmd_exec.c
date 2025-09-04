/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amsagman <amsagman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 16:32:07 by sedemir           #+#    #+#             */
/*   Updated: 2025/08/24 04:51:20 by amsagman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**build_cmd_args(char *cmd, char **env, char **args)
{
	char	**first_args;
	char	**full_args;

	first_args = prepare_cmd_arguments(cmd, env, 0);
	full_args = combine_command_arguments(first_args, args);
	return (full_args);
}

static char	**prepare_and_update_env(char **cmd, t_env *env)
{
	char	**cmd_args;
	int		last_idx;
	char	*env_var;

	last_idx = 0;
	cmd_args = build_cmd_args(cmd[0], env->original_env, cmd);
	while (cmd_args[last_idx])
		last_idx++;
	if (last_idx > 0)
	{
		env_var = ft_strjoin("_=", cmd_args[last_idx - 1]);
		update_environment_var(env_var, env);
	}
	return (cmd_args);
}

static int	exe_prepared_cmd(char **cmd_args, int *fd, int *piped, t_env *env)
{
	int	status;

	if (check_if_command_is_builtin(cmd_args[0]))
		status = execute_builtin(cmd_args, fd, env, piped);
	else
	{
		piped[PIPE_CHILD_COUNT]++;
		if (!piped[PIPE_REDIR_COMBO])
			status = run_basic_cmd(cmd_args, fd, env->original_env, piped);
		else
			status = run_redir_cmd(cmd_args, fd, env->original_env, piped);
	}
	if (piped[PIPE_ACTIVE] > 1)
		piped[PIPE_ACTIVE]--;
	return (status);
}

int	prep_run_cmd(char **cmd, int *fd, int *piped, t_env *env)
{
	char	**cmd_args;
	int		status;

	cmd_args = prepare_and_update_env(cmd, env);
	status = exe_prepared_cmd(cmd_args, fd, piped, env);
	return (status);
}

int	wait_children(int status, int *_piped)
{
	if (status != 2 && status != 127 && status != 126
		&& _piped[PIPE_CHILD_COUNT] && _piped[PIPE_EXEC_STATE])
	{
		while (_piped[PIPE_CHILD_COUNT])
		{
			wait(&status);
			_piped[PIPE_CHILD_COUNT] -= 1;
		}
		signal(SIGINT, handle_ctrl_c);
		signal(SIGQUIT, SIG_IGN);
		if (!g_var_thing)
			return (WEXITSTATUS(status));
		else
			return (g_var_thing);
	}
	return (status);
}
