/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_sh.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjusta <mjusta@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 16:25:28 by dkolarov          #+#    #+#             */
/*   Updated: 2025/11/23 03:12:50 by mjusta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cleanup_sh.h"
#include "libft_cleanup.h"
#include "env.h"
#include "tokenizer.h"
#include "parser.h"
#include <stdlib.h>

/**
 * @brief Free entire enviroment list.
 */
void	env_free(t_env *env)
{
	t_env	*next;

	while (env)
	{
		next = env->next;
		free(env->key);
		free(env->value);
		free(env);
		env = next;
	}
}

void	cleanup_shell(t_shell *sh)
{
	if (!sh)
		return ;
	if (sh->path_dirs)
		free_char_arr(sh->path_dirs);
	if (sh->env)
		env_free(sh->env);
	if (sh->ast)
		ast_free(sh->ast);
	if (sh->tokens)
		free_tokens(sh->tokens);
}

/**
 * @brief Free a linked list of tokens produced by the lexer.
 *
 * This function releases both the node and the `value` string for each token.
 *
 * @param token Head of the token list (may be NULL).
 */
void	free_tokens(t_tok *token)
{
	t_tok	*tmp;

	while (token)
	{
		tmp = token->next;
		free(token->value);
		free(token);
		token = tmp;
	}
}
