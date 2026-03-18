/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjusta <mjusta@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 21:30:05 by mjusta            #+#    #+#             */
/*   Updated: 2025/11/26 01:17:56 by mjusta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "tokenizer.h"
#include "libft_output.h"

/**
 * @brief Handle an operator (|, <, >, <<, >>) at the current index.
 *
 * Emits the appropriate operator token and advances `*i` by 1 or 2 characters.
 *
 * @param tokens Pointer to the token list head.
 * @param s      Full source string.
 * @param i      In/out index into `s`.
 */
static void	handle_operator_token(t_tok **tokens, const char *s, size_t *i)
{
	t_token_type	type;
	size_t			len;
	char			c1;
	char			c2;

	type = get_operator_type(&s[*i]);
	c1 = s[*i];
	c2 = s[*i + 1];
	len = 1;
	if ((c1 == '<' && c2 == '<') || (c1 == '>' && c2 == '>'))
		len = 2;
	add_token(tokens, new_token(type, &s[*i], len));
	*i += len;
}

/**
  * @brief Parse one logical word at the current index
 *
 * @param tokens Pointer to the token list head.
 * @param s      Full source string.
 * @param i      In/out index into `s`.
 * 
 * @return 0 on success, 1 on error
 */
static int	handle_word_token(t_tok **tokens, const char *s, size_t *i)
{
	t_scan	scan;
	t_tok	*tok;
	size_t	start;

	start = *i;
	if (word_measure(s, start, &scan) != 0)
		return (1);
	tok = new_token(T_STRING, s + start, scan.len);
	if (!tok)
		return (1);
	add_token(tokens, tok);
	*i = scan.end;
	return (0);
}

/**
 * @brief Lexically analyze a shell-like input line into a linked list of 
 * tokens.
 * 
 * Behavior:
 *  - Skips leading whitespace.
 *  - Emits operators/metacharacters: |, <, >, <<, >>.
 *  - Otherwise parses one *logical word*: merges unquoted bytes with the inner
 *    contents of '...' and "..." into a single T_STRING; quote characters are
 *    stripped. If a quote is not closed on this line, an error is reported.
 * 
 * @return 0 on success. 1 on error.
 */
int	lex_line(const char *line, t_tok **tokens)
{
	size_t	i;

	*tokens = NULL;
	i = 0;
	while (line[i])
	{
		skip_whitespace(line, &i);
		if (!line[i])
			break ;
		if (is_metachar(line[i]))
			handle_operator_token(tokens, line, &i);
		else if (handle_word_token(tokens, line, &i) != 0)
		{
			ft_putstr_fd("minishell: syntax error: unterminated quote\n", 2);
			return (1);
		}
	}
	return (0);
}
