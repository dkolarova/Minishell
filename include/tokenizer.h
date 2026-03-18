/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjusta <mjusta@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 17:24:38 by mjusta            #+#    #+#             */
/*   Updated: 2025/11/21 15:07:29 by mjusta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZER_H
# define TOKENIZER_H

# include <stddef.h>

typedef enum e_token_type
{
	T_STRING,
	T_PIPE,
	T_REDIR_IN,
	T_REDIR_OUT,
	T_HEREDOC,
	T_APPEND
}	t_token_type;

typedef struct s_scan
{
	size_t		end;
	size_t		len;
}	t_scan;

typedef struct s_tok
{
	t_token_type	type;
	char			*value;
	int				heredoc;
	struct s_tok	*next;
}	t_tok;

int		word_measure(const char *s, size_t i, t_scan *scan);
t_tok	*new_token(t_token_type type, const char *s, size_t len);
void	add_token(t_tok **head, t_tok *new);

#endif