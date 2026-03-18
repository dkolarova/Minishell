/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjusta <mjusta@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 22:24:53 by mjusta            #+#    #+#             */
/*   Updated: 2025/11/26 01:15:50 by mjusta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "executor.h"
#include "libft_string.h"
#include "lexer.h"
#include "tokenizer.h"
#include <stdlib.h>

/**
 * @brief appends the word to the commands argv array
 * 
 * Allocates new argv array, copies existing values in it and appends new word
 * and replaces the old pointer.
 * 
 * @return 0 on success and -1 on allocation failure
 */
static int	append_word_cmd(t_cmd *cmd, const char *s)
{
	char	**old;
	char	**new;
	size_t	n;
	size_t	i;

	old = cmd->argv;
	n = argcount(old);
	new = (char **)malloc(sizeof(char *) * (n + 2));
	if (!new)
		return (-1);
	i = 0;
	while (i < n)
	{
		new[i] = old[i];
		i++;
	}
	new[n] = ft_strdup(s);
	if (!new[n])
		return (free(new), -1);
	new[n + 1] = NULL;
	free(old);
	cmd->argv = new;
	return (0);
}

/**
 * @brief Parse a single simple command until a pipe or end of tokens.
 * 
 * Recognizes words, strings, and redirections. Each valid word is added to
 * argv, and redirections are attached to the command node. Stops at a pipe.
 * 
 * @return 0 on Success. 1 if error
 */
static int	parse_simple_command(t_tok **token, t_cmd *cmd)
{
	while (*token && (*token)->type != T_PIPE)
	{
		if (is_redir_tok(*token))
		{
			if (parse_one_redir(token, cmd) < 0)
				return (1);
		}
		else if ((*token)->type == T_STRING)
		{
			if (append_word_cmd(cmd, (*token)->value) < 0)
				return (1);
			*token = (*token)->next;
		}
		else
			return (1);
	}
	if (!(cmd->argv || cmd->redirs))
		return (1);
	return (0);
}

/**
 * @brief Consumes token and builds an AST of commands/
 * 
 * @param token first token to parse
 * @param head Head of the ast.
 * 
 * @return 0 success. 1 error
 */
static int	build_ast(t_tok *token, t_cmd **head)
{
	t_tok	*current;
	t_cmd	*cmd;	
	t_cmd	*tail;

	tail = NULL;
	current = token;
	while (current)
	{
		cmd = (t_cmd *)ft_calloc(1, sizeof(t_cmd));
		if (!cmd)
			return (1);
		if (parse_simple_command(&current, cmd) != 0)
		{
			ast_free(cmd);
			return (1);
		}
		if (!*head)
			*head = cmd;
		else
			tail->next = cmd;
		tail = cmd;
		if (current && current->type == T_PIPE)
			current = current->next;
	}
	return (0);
}

/**
 * @brief Parse a token list into an abstract syntax tree (AST) of commands.
 * 
 * @return 0 on success; 1 on error
 */
int	parse_tokens(t_tok *tokens, t_ast **ast)
{
	t_ast	*head;

	*ast = NULL;
	if (!tokens)
		return (0);
	if (check_pipe_syntax(tokens))
		return (1);
	head = NULL;
	if (build_ast(tokens, &head) != 0)
	{
		ast_free(head);
		return (1);
	}
	*ast = head;
	return (0);
}
