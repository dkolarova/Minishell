/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjusta <mjusta@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 01:26:33 by mjusta            #+#    #+#             */
/*   Updated: 2025/11/21 17:04:27 by mjusta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expander.h"
#include "tokenizer.h"
#include "libft_string.h"
#include "env.h"
#include <stdlib.h>

/**
 * @brief  Expand a single environment variable.
 *
 * @return Newly allocated expanded value, or NULL on error.
 */
static char	*expand_env_var(const char *s, size_t *i, t_shell *shell)
{
	size_t	start;
	char	*name;
	char	*result;

	(*i)++;
	if (s[*i] == '?')
	{
		(*i)++;
		return (ft_itoa(shell->last_status));
	}
	if (!s[*i] || !(ft_isalpha(s[*i]) || s[*i] == '_'))
		return (ft_strdup("$"));
	start = *i;
	while (s[*i] && (ft_isalnum(s[*i]) || s[*i] == '_'))
		(*i)++;
	name = ft_substr(s, start, *i - start);
	if (!name)
		return (NULL);
	result = env_get(shell->env, name);
	free(name);
	if (!result)
		return (ft_strdup(""));
	return (ft_strdup(result));
}

/**
 * @brief  Update quote state based on current character.
 * 
 * @return  1 if quote state was toggled, 0 otherwise.
 */
static int	is_quote_toggle(char c, t_quote *q)
{
	if (c == '\'' && !q->doubleq)
	{
		q->singleq = !q->singleq;
		return (1);
	}
	else if (c == '"' && !q->singleq)
	{
		q->doubleq = !q->doubleq;
		return (1);
	}
	return (0);
}

/**
 * @brief  Core expansion loop for a single word.
 * 
 * @return 0 on success, 1 on allocation error.
 */
static int	expand_loop(const char *s, t_shell *shell, t_quote *q, char **out)
{
	size_t	i;
	char	*tmp;

	i = 0;
	while (s[i])
	{
		if (is_quote_toggle(s[i], q))
			i++;
		else if (s[i] == '$' && !q->singleq)
		{
			tmp = expand_env_var(s, &i, shell);
			if (!tmp || append_string(out, tmp))
				return (free(tmp), 1);
			free(tmp);
		}
		else
		{
			if (append_char(out, s[i]))
				return (1);
			i++;
		}
	}
	return (0);
}

/**
 * @brief  Expand a single word token.
 * 
 * @return Newly allocated expanded string, or NULL on error.
 */
static char	*expand_word(const char *s, t_shell *shell)
{
	char	*out;
	t_quote	q;

	out = NULL;
	q.singleq = 0;
	q.doubleq = 0;
	if (expand_loop(s, shell, &q, &out) != 0)
		return (free(out), NULL);
	if (!out)
		return (ft_strdup(""));
	return (out);
}

/**
 * @brief  Expand all string tokens in the token list.
 * 
 * @return 0 on success, 1 on allocation error.
 */
int	expand_tokens(t_shell *shell)
{
	t_tok	*tok;
	t_tok	*prev;
	char	*new;

	tok = shell->tokens;
	prev = NULL;
	while (tok)
	{
		if (tok->type == T_STRING)
		{
			if (prev && prev->type == T_HEREDOC)
			{
				if (ft_strchr(tok->value, '\'' ) || ft_strchr(tok->value, '"'))
					tok->heredoc = 1;
			}
			new = expand_word(tok->value, shell);
			if (!new)
				return (1);
			free(tok->value);
			tok->value = new;
		}
		prev = tok;
		tok = tok->next;
	}
	return (0);
}
