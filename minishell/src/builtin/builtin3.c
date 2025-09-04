/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amsagman <amsagman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 17:02:22 by sedemir           #+#    #+#             */
/*   Updated: 2025/08/24 05:19:57 by amsagman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_session(char **_cmd_)
{
	int	status;

	status = 0;
	ft_putendl_fd("exit", STDOUT_FILENO);
	if (_cmd_[1] && _cmd_[2])
	{
		ft_putendl_fd("exit: too many arguments", STDERR_FILENO);
		status = 1;
	}
	else if (_cmd_[1] && !is_numeric_string(_cmd_[1]))
	{
		ft_putstr_fd("exit: ", STDERR_FILENO);
		ft_putstr_fd(_cmd_[1], STDERR_FILENO);
		ft_putendl_fd(": numeric argument required", STDERR_FILENO);
		status = 2;
	}
	else if (_cmd_[1])
		status = strtoint(_cmd_[1]);
	clear_garbage();
	exit(status & 0xFF);
}

int	is_export_with_args(char **cmd)
{
	int	a;

	a = 1;
	while (cmd[0] && cmd[a])
	{
		if (cmd[a])
			return (1);
		a++;
	}
	return (0);
}

int	find_equal_pos(char *cmd, int len)
{
	int	i;

	i = 0;
	while (i < len)
	{
		if (cmd[i] == '=')
			return (i);
		i++;
	}
	return (-1);
}

int	validate_var_name(char *cmd, int var_len)
{
	int	i;

	if (var_len == 0)
		return (0);
	if (!ft_isalpha(cmd[0]) && cmd[0] != '_')
		return (0);
	i = 1;
	while (i < var_len)
	{
		if (!ft_isalnum(cmd[i]) && cmd[i] != '_')
			return (0);
		i++;
	}
	return (1);
}
