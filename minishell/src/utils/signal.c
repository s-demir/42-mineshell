/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amsagman <amsagman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 15:09:51 by amsagman          #+#    #+#             */
/*   Updated: 2025/08/17 05:48:37 by amsagman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	configure_signal_handlers(void)
{
	signal(SIGINT, handle_ctrl_c);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGQUIT, handle_ctrl_backslash);
}

void	child_ctrl_c(int sig_num)
{
	if (sig_num == 2)
		g_var_thing = 130;
	else
	{
		ft_putstr_fd("Quit (core dumped)", 1);
		g_var_thing = 131;
	}
	write(1, "\n", 1);
}

void	handle_ctrl_c(int a)
{
	(void)a;
	g_var_thing = 1;
	rl_replace_line("", 0);
	write(1, "\n", 1);
	rl_on_new_line();
	rl_redisplay();
}

void	handle_ctrl_backslash(int sig)
{
	(void)sig;
	if (g_var_thing == 0)
	{
		ft_putstr_fd("\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else
		ft_putstr_fd("Quit (core dumped)", 1);
}
