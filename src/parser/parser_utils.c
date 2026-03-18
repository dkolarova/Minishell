/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjusta <mjusta@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 22:03:08 by mjusta            #+#    #+#             */
/*   Updated: 2025/11/21 17:05:34 by mjusta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "executor.h"
#include "tokenizer.h"
#include <stdlib.h>

/**
 * @brief cleans up list of redirections from cmd.
 */
void	free_redirs(t_redir *r)
{
	t_redir	*temp;

	while (r)
	{
		temp = r->pipe_next;
		free(r->arg);
		free(r);
		r = temp;
	}
}

/**
 * @brief cleans up the asynchronus tree from memory.
 */
void	ast_free(t_ast *ast)
{
	t_cmd	*next;

	while (ast)
	{
		next = ast->next;
		free_char_arr(ast->argv);
		free_redirs(ast->redirs);
		free(ast);
		ast = next;
	}
}

/*
 * Free resources that belong to a single command node (argv array and
 * associated redirections). This is intended for use in forked child
 * processes that should free command-local memory before calling exit to
 * avoid Valgrind reporting still-reachable blocks in the child.
 */
void	free_cmd_resources(t_cmd *cmd)
{
	if (!cmd)
		return ;
	free_char_arr(cmd->argv);
	free_redirs(cmd->redirs);
}

/**
 * @brief counts the number of arguments in array
 */
size_t	argcount(char **argv)
{
	size_t	i;

	if (!argv)
		return (0);
	i = 0;
	while (argv[i])
		i++;
	return (i);
}

int	check_pipe_syntax(t_tok *token)
{
	t_tok	*prev;

	prev = NULL;
	while (token)
	{
		if (token->type == T_PIPE)
		{
			if (!prev || (token->next && token->next->type == T_PIPE))
			{
				ft_putstr_fd(
					"minishell: syntax error near unexpected token `|'\n", 2);
				return (1);
			}
			if (!token->next)
			{
				ft_putstr_fd(
					"minishell: syntax error near unexpected token `newline'\n",
					2);
				return (1);
			}
		}
		prev = token;
		token = token->next;
	}
	return (0);
}
