/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exec_init2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amsagman <amsagman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 16:55:40 by sedemir           #+#    #+#             */
/*   Updated: 2025/08/16 23:33:30 by amsagman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_directory_status(char *path_, char *file, int *status)
{
	struct stat	s;

	if (file && str_cmp(file, ".", NULL))
		*status = 2;
	else if (str_cmp(file, "..", NULL) || str_cmp(file, ",", ""))
	{
		*status = 1;
	}
	else if (!stat(path_, &s) && S_ISDIR(s.st_mode))
	{
		*status = 2;
		ft_putstr_fd("err: ", 2);
		ft_putstr_fd(path_, 2);
		ft_putendl_fd(": Is a directory", 2);
	}
}

int	report_command_error(char *file, int _status)
{
	if (_status == 1)
	{
		_status = get_exit_status(2);
		ft_putstr_fd("err: ", 2);
		ft_putstr_fd(file, 2);
		ft_putstr_fd(": No such file or directory", 2);
		ft_putchar_fd('\n', 2);
		return (_status);
	}
	else if (_status == 127 || _status == 2)
	{
		if (ft_strcmp(file, "/") == 0 && _status == 2)
			return (126);
		ft_putstr_fd(file, 2);
		ft_putendl_fd(": command not found", 2);
		return (127);
	}
	return (_status);
}

void	verify_path(t_ast_node *head, char **env, int *status)
{
	char		*path;
	struct stat	s;

	path = find_cmd_path(head->args[0], env, "PWD", R_OK);
	if (!path)
	{
		if (head->args[0][0] == '/' || head->args[0][0] == '.')
		{
			if (stat(head->args[0], &s) == -1)
				*status = 1;
			else
				*status = 127;
		}
		else
			*status = 127;
	}
	else
		check_directory_status(path, head->args[0], status);
	*status = report_command_error(head->args[0], *status);
}

int	check_cmd_perms(t_ast_node *head, char **env)
{
	int	status;

	status = 0;
	if (head->args && !check_if_command_is_builtin(head->args[0])
		&& head->file_type == EXECUTE_FILE)
	{
		if (head->args[0][0] == '/' || head->args[0][0] == '.')
		{
			verify_path(head, env, &status);
		}
	}
	if (!status && head->left)
		status = check_cmd_perms(head->left, env);
	if (!status && head->right)
		status = check_cmd_perms(head->right, env);
	return (status);
}
