/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sedemir <sedemir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 15:57:44 by sedemir           #+#    #+#             */
/*   Updated: 2025/07/31 15:57:58 by sedemir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	new_quote_state(char c, int *single_q, int *double_q)
{
	if (c == 34)
		(*double_q)++;
	else if (c == 39)
		(*single_q)++;
}

const char	*is_spaces(const char *input)
{
	int	i;

	i = 0;
	while (input[i] && (input[i] == ' ' || input[i] == '\t'))
		i++;
	return (input + i);
}

int	is_invalid_operator(const char **input)
{
	const char	*operator_start;

	operator_start = *input;
	(*input)++;
	if (*operator_start == **input)
		(*input)++;
	*input = is_spaces(*input);
	if (**input == '\0' || **input == '>' || **input == '<' || **input == '|')
		return (1);
	return (0);
}
