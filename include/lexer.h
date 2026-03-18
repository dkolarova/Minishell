/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjusta <mjusta@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 21:21:44 by mjusta            #+#    #+#             */
/*   Updated: 2025/11/21 16:50:28 by mjusta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include <stddef.h>

typedef struct s_tok		t_tok;
typedef enum e_token_type	t_token_type;

int				lex_line(const char *line, t_tok **tokens);

void			skip_whitespace(const char *s, size_t *i);
int				is_metachar(char c);
int				is_quote(char c);
t_token_type	get_operator_type(const char *s);

#endif
