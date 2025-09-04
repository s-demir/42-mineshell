/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sedemir <sedemir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 16:44:49 by sedemir           #+#    #+#             */
/*   Updated: 2025/07/31 16:45:16 by sedemir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	compare_strings(char *s1, char *s2, int max)
{
	int	index;

	index = 0;
	while (index < max && s1[index] && s2[index] && s1[index] == s2[index])
		index++;
	if (string_length(s1, '\0') == index && index == max)
		return (1);
	return (0);
}

void	exit_heredoc(int sig)
{
	(void)sig;
	close(get_env()->out_fd);
	write(STDOUT_FILENO, "\n", 1);
	clear_garbage();
	exit(0);
}

int	has_quotes(char *s)
{
	int	index;

	index = 0;
	while (s && s[index])
	{
		if (s[index] == 34 || s[index] == 39)
			return (0);
		index++;
	}
	return (1);
}
