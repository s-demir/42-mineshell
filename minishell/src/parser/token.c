/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sedemir <sedemir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 16:04:51 by sedemir           #+#    #+#             */
/*   Updated: 2025/08/12 16:32:04 by sedemir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_special(char **input, t_token **tokens)
{
	if (**input == '>')
	{
		if (*(*input + 1) == '>')
		{
			append_token(tokens, create_token(TOKEN_REDIR_APPEND, ">>"));
			(*input)++;
		}
		else
			append_token(tokens, create_token(TOKEN_REDIR_OUT, ">"));
	}
	else if (**input == '<')
	{
		if (*(*input + 1) == '<')
		{
			append_token(tokens, create_token(TOKEN_REDIR_HEREDOC, "<<"));
			(*input)++;
		}
		else
			append_token(tokens, create_token(TOKEN_REDIR_IN, "<"));
	}
	else if (**input == '|')
		append_token(tokens, create_token(TOKEN_PIPE, "|"));
	(*input)++;
}

void	handle_word(char **input, t_token **tokens)
{
	char	*start;
	int		in_quote;
	char	quote_char;

	start = *input;
	in_quote = 0;
	quote_char = '\0';
	while (**input)
	{
		update_quote_status(**input, &in_quote, &quote_char);
		if (!in_quote && ft_strchr(" \t\n><|", **input))
			break ;
		(*input)++;
	}
	add_word_token_if_valid(&start, input, tokens);
}

t_token	*tokenize_input(char *input)
{
	t_token	*tokens;
	char	*ptr;

	tokens = NULL;
	ptr = input;
	while (*ptr)
	{
		if (ft_isspace(*ptr))
		{
			ptr++;
			continue ;
		}
		if (ft_strchr("><|", *ptr))
			handle_special(&ptr, &tokens);
		else
			handle_word(&ptr, &tokens);
		if (!*ptr)
			break ;
	}
	return (tokens);
}
