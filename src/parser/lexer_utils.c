/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjusta <mjusta@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 22:59:57 by mjusta            #+#    #+#             */
/*   Updated: 2025/11/21 17:02:12 by mjusta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "libft_char.h"
#include "tokenizer.h"

void	skip_whitespace(const char *s, size_t *i)
{
	while (ft_isspace(s[*i]))
		(*i)++;
}

int	is_metachar(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

int	is_quote(char c)
{
	return (c == '\'' || c == '"');
}

/**
 *  * @brief Map the operator substring at `s` to a token type.
 *
 * Recognizes `<<` (heredoc), `>>` (append), `<` (redir in), `>` (redir out),
 * and `|` (pipe). Falls back to T_PIPE if none match (defensive).
 * 
 * @return Resolved operator token type.
 */
t_token_type	get_operator_type(const char *s)
{
	if (s[0] == '<' && s[1] == '<')
		return (T_HEREDOC);
	if (s[0] == '>' && s[1] == '>')
		return (T_APPEND);
	if (s[0] == '<')
		return (T_REDIR_IN);
	if (s[0] == '>')
		return (T_REDIR_OUT);
	return (T_PIPE);
}
