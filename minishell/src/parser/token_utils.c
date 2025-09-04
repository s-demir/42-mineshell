/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sedemir <sedemir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 16:06:04 by sedemir           #+#    #+#             */
/*   Updated: 2025/08/12 16:32:35 by sedemir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*create_token(t_token_type type, char *value)
{
	t_token	*token;

	token = galloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->value = ft_strdup(value);
	if (!token->value)
	{
		return (NULL);
	}
	token->next = NULL;
	return (token);
}

void	append_token(t_token **tokens, t_token *new_token)
{
	t_token	*last;

	if (!new_token)
		return ;
	if (!*tokens)
		*tokens = new_token;
	else
	{
		last = *tokens;
		while (last->next)
			last = last->next;
		last->next = new_token;
	}
}

void	clear_tokens(t_token *tokens)
{
	t_token	*current;
	t_token	*next;

	current = tokens;
	while (current)
	{
		next = current->next;
		current = next;
	}
}

void	update_quote_status(char c, int *in_quote, char *quote_char)
{
	if (!*in_quote && (c == '\'' || c == '\"'))
	{
		*in_quote = 1;
		*quote_char = c;
	}
	else if (*in_quote && c == *quote_char)
		*in_quote = 0;
}

void	add_word_token_if_valid(char **start, char **input, t_token **tokens)
{
	char	*word;
	t_token	*new_token;

	if (*input > *start)
	{
		word = ft_strndup(*start, *input - *start);
		if (word)
		{
			new_token = create_token(TOKEN_WORD, word);
			if (new_token)
				append_token(tokens, new_token);
			else
				ft_putstr_fd("Error: Token creation failed.\n", 2);
		}
		else
			ft_putstr_fd("Error: galloc failed in handle_word.\n", 2);
	}
}
