/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjusta <mjusta@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 17:24:20 by mjusta            #+#    #+#             */
/*   Updated: 2025/11/21 16:49:39 by mjusta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"
#include "libft_string.h"
#include "libft_char.h"
#include "lexer.h"
#include <stdlib.h>

/**
 * @brief Allocate and initialize a new token node.
 * 
 * Copies `len` bytes from `start` into a freshly allocated, null-terminated
 * string and stores it in `token->value`.
 * 
 * @return Newly allocated token, or NULL if allocation fails.
 */
t_tok	*new_token(t_token_type type, const char *s, size_t len)
{
	t_tok	*token;

	token = (t_tok *)malloc(sizeof(t_tok));
	if (!token)
		return (NULL);
	token->value = ft_substr(s, 0, len);
	if (!token->value)
	{
		free(token);
		return (NULL);
	}
	token->type = type;
	token->next = NULL;
	return (token);
}

/**
 * @brief Append a token to the end of a singly-linked token list.
 *
 * If `*head` is NULL, the new token becomes the head.
 *
 */
void	add_token(t_tok **head, t_tok *new)
{
	t_tok	*tmp;

	if (!new)
		return ;
	if (!*head)
		*head = new;
	else
	{
		tmp = *head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

/**
 * @brief Find the matching closing quotes.
 * 
 * @param s input string
 * @param i index of the first quote
 * @param q quote char to match
 * 
 * @return index of the closing quote or -1 on error.
 */
static size_t	find_closing_quotes(const char *s, size_t i, char q)
{
	size_t	j;

	j = i + 1;
	while (s[j] && s[j] != q)
		j++;
	if (s[j] == '\0')
		return (-1);
	return (j);
}

/**
 * @brief Compute raw len, end, and qclass.
 * 
 * Word ends on unquoted space or metachar. 
 * 
 * @return 0 on success, 1 if an unterminated quote is found.
 */
int	word_measure(const char *s, size_t i, t_scan *scan)
{
	size_t	start;
	size_t	j;

	scan->len = 0;
	start = i;
	while (s[i])
	{
		if (ft_isspace(s[i]) || is_metachar(s[i]))
			break ;
		if (is_quote(s[i]))
		{
			j = find_closing_quotes(s, i, s[i]);
			if (j == (size_t)(-1))
				return (1);
			i = j + 1;
		}
		else
			i++;
	}
	scan->end = i;
	scan->len = scan->end - start;
	return (0);
}
