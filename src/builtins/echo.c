/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkolarov <dkolarov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 09:36:37 by dkolarov          #+#    #+#             */
/*   Updated: 2025/12/05 11:33:47 by dkolarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtins.h"

static int	is_n_flag(const char *s);
static int	parse_n_flags(char **argv, int *idx);

/**
 * @brief Detect builtin command type from command name
 * 
 * @param cmd Command name (argv[0])
 * @return t_builtin Builtin type or B_NONE if not a builtin
 */
t_builtin	builtin_from(const char *cmd)
{
	if (!cmd)
		return (B_NONE);
	if (ft_strncmp(cmd, "echo", 5) == 0)
		return (B_ECHO);
	if (ft_strncmp(cmd, "cd", 3) == 0)
		return (B_CD);
	if (ft_strncmp(cmd, "pwd", 4) == 0)
		return (B_PWD);
	if (ft_strncmp(cmd, "export", 7) == 0)
		return (B_EXPORT);
	if (ft_strncmp(cmd, "unset", 6) == 0)
		return (B_UNSET);
	if (ft_strncmp(cmd, "env", 4) == 0)
		return (B_ENV);
	if (ft_strncmp(cmd, "exit", 5) == 0)
		return (B_EXIT);
	return (B_NONE);
}

/**
 * @brief Check if builtin modifies shell state (needs to run in parent)
 * 
 * @param id Builtin type
 * @return int 1 if stateful, 0 if can run in child process
 */
int	is_stateful_builtin(t_builtin id)
{
	return (id == B_CD || id == B_EXPORT || id == B_UNSET || id == B_EXIT);
}

/** 
 * @brief Check if string is a "-n"-style flag
 * 
 * Returns 1 if `s` matches "-n", "-nn", ... exactly; otherwise 0.
 * 
 * @param s Input string
 * @return int 1 if string is a -n flag, 0 otherwise
 */
static int	is_n_flag(const char *s)
{
	int	j;

	if (!s || s[0] != '-')
		return (0);
	j = 1;
	if (s[j] == '\0')
		return (0);
	while (s[j] == 'n')
		j++;
	return (s[j] == '\0');
}

/** 
 * @brief Consume consecutive "-n"-style flags in argv
 * 
 * Starts at argv[*idx] and advances *idx past all consecutive -n flags.
 * Returns 0 if a newline should be suppressed (i.e., at least one -n flag
 * was found), or 1 if no -n flags were found.
 * 
 * @param argv Argument vector
 * @param idx Pointer to current index in argv (will be updated)
 * @return int 0 if newline should be suppressed, 1 otherwise
 */
static int	parse_n_flags(char **argv, int *idx)
{
	int	newline;

	newline = 1;
	while (argv[*idx] && argv[*idx][0] == '-')
	{
		if (!is_n_flag(argv[*idx]))
			break ;
		newline = 0;
		(*idx)++;
	}
	return (newline);
}

/**
 * @brief Implement echo builtin
 * 
 * @param argv Command arguments
 * @return int Exit status
 */
int	bi_echo(char **argv)
{
	int	i;
	int	newline;

	if (!argv)
		return (1);
	newline = 1;
	i = 1;
	newline = parse_n_flags(argv, &i);
	while (argv[i])
	{
		ft_putstr_fd(argv[i], 1);
		if (argv[i + 1])
			ft_putchar_fd(' ', 1);
		i++;
	}
	if (newline)
		ft_putchar_fd('\n', 1);
	return (0);
}
