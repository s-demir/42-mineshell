/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sedemir <sedemir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 15:59:58 by sedemir           #+#    #+#             */
/*   Updated: 2025/07/31 16:04:20 by sedemir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	unclosed_quotes(const char *input)
{
	int		quote_i;
	char	quote_char;
	size_t	i;

	quote_i = 0;
	quote_char = 0;
	i = 0;
	while (input[i])
	{
		if (!quote_i && (input[i] == '\'' || input[i] == '\"'))
		{
			quote_i = 1;
			quote_char = input[i];
		}
		else if (quote_i && input[i] == quote_char)
			quote_i = 0;
		i++;
	}
	return (quote_i != 0);
}

static int	invalid_redirection(const char *input)
{
	int		single_q;
	int		double_q;
	size_t	i;
	size_t	j;

	single_q = 0;
	double_q = 0;
	i = 0;
	while (input[i++])
	{
		new_quote_state(input[i], &single_q, &double_q);
		if ((single_q % 2 == 0 && double_q % 2 == 0) && (input[i] == '>'
				|| input[i] == '<'))
		{
			j = i + 1;
			if (input[j] == input[i])
				j++;
			while (input[j] && (input[j] == ' ' || input[j] == '\t'))
				j++;
			if (!input[j] || input[j] == '>' || input[j] == '<'
				|| input[j] == '|')
				return (1);
		}
	}
	return (0);
}

static int	misplaced_operator(const char *input)
{
	int		expect_command;
	int		single_q;
	int		double_q;
	size_t	i;

	expect_command = 0;
	single_q = 0;
	double_q = 0;
	i = 0;
	if (input[0] == '|' || input[0] == '&')
		return (1);
	while (input[i])
	{
		new_quote_state(input[i], &single_q, &double_q);
		if (input[i] == '|' && single_q % 2 == 0 && double_q % 2 == 0)
		{
			if (expect_command)
				return (1);
			expect_command = 1;
		}
		else if (!ft_isspace(input[i]))
			expect_command = 0;
		i++;
	}
	return (expect_command);
}

static int	unsupported_operators(const char *input)
{
	int		single_q;
	int		double_q;
	size_t	i;

	single_q = 0;
	double_q = 0;
	i = 0;
	while (input[i])
	{
		new_quote_state(input[i], &single_q, &double_q);
		if (single_q % 2 == 0 && double_q % 2 == 0 && ((input[i] == '&'
					&& input[i + 1] == '&') || (input[i] == '|' && input[i
						+ 1] == '|')))
			return (1);
		i++;
	}
	return (0);
}

int	check_syntax(const char *input)
{
	if (unclosed_quotes(input))
		return (ft_putstr_fd("Syntax error: unclosed quote\n", 2), 1);
	if (invalid_redirection(input))
		return (ft_putstr_fd("Syntax error: invalid redirection\n", 2), 1);
	if (misplaced_operator(input))
		return (ft_putstr_fd("Syntax error: misplaced operator\n", 2), 1);
	if (unsupported_operators(input))
		return (ft_putstr_fd
			("Error: Logical operators '&&' and '||' are not supported.\n",
				2), 1);
	return (0);
}
