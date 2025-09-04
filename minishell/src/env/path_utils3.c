/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amsagman <amsagman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 12:19:04 by amsagman          #+#    #+#             */
/*   Updated: 2025/08/24 12:19:08 by amsagman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*resolve_command_path(char *cmd_holder, char **envp)
{
	char	*cmd_path;
	char	*found_cmd;

	cmd_path = find_cmd_path(cmd_holder, envp, "PATH", X_OK);
	if (!cmd_path)
		return (ft_strdup(cmd_holder));
	found_cmd = strrchr(cmd_path, '/');
	if (found_cmd && found_cmd[1] == '\0')
		return (ft_strdup(cmd_holder));
	if (found_cmd)
	{
		found_cmd++;
		if (!str_cmp(found_cmd, cmd_holder, NULL))
			return (ft_strdup(cmd_holder));
	}
	return (cmd_path);
}

static char	*process_command_token(char *cmd, char **envp, int *i, int c)
{
	char	*cmd_holder;

	cmd_holder = extract_next_substring(cmd, ' ', i);
	if (!c && !check_if_command_is_builtin(cmd_holder))
		return (resolve_command_path(cmd_holder, envp));
	return (cmd_holder);
}

char	**prepare_cmd_arguments(char *cmd, char **envp, int c)
{
	char	**cmd_arr;
	int		i[3];
	int		token_count;

	token_count = count_tokens(cmd, ' ');
	cmd_arr = galloc((token_count + 1) * sizeof(char *));
	i[0] = 0;
	while (c < token_count)
	{
		cmd_arr[c] = process_command_token(cmd, envp, i, c);
		if (!cmd_arr[c])
			return (NULL);
		c++;
	}
	cmd_arr[c] = NULL;
	return (cmd_arr);
}
