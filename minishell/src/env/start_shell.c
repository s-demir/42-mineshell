/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_shell.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amsagman <amsagman@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 15:12:20 by amsagman          #+#    #+#             */
/*   Updated: 2025/08/12 22:45:58 by amsagman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	update_pwd(t_env *env, int idx)
{
	char	*pwd;

	pwd = get_current_working_directory(100, 5, 2);
	if (pwd)
	{
		if (idx >= 0)
			remove_env_entry(env, idx);
		update_pwd_env(pwd, env, idx);
	}
}

static void	set_defaults(t_env *env, int idx)
{
	idx = locate_env_var_index(env, "SHELL");
	if (idx >= 0)
		remove_env_entry(env, idx);
	update_environment_var("SHELL=minishell", env);
	update_environment_var("?=0", env);
	idx = locate_env_var_index(env, "PWD");
	update_pwd(env, idx);
}

static void	update_shlvl(t_env *env, int idx)
{
	int	value;

	value = 0;
	if (idx >= 0)
		value = strtoint(env->parsed_env[idx][1]);
	set_env_status(env, value + 1, "SHLVL=");
}

int	start_shell(t_env *env, char **orig_env)
{
	int	status;
	int	idx;

	if (!env)
		return (0);
	status = setup_env(env, orig_env, 0);
	idx = locate_env_var_index(env, "SHLVL");
	update_shlvl(env, idx);
	set_defaults(env, 0);
	return (status);
}
