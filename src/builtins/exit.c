/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjusta <mjusta@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 10:39:13 by dkolarov          #+#    #+#             */
/*   Updated: 2025/11/23 04:36:55 by mjusta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtins.h"
#include "cleanup_sh.h"

static int	validate_numeric(char *arg);
static int	compute_exit_status(char *arg);

/**
 * @brief Implement exit builtin
 * 
 * @param argv Command arguments
 * @param sh Shell state
 * @return int Exit status
 */
int	bi_exit(char **argv, t_shell *sh)
{
	if (isatty(STDOUT_FILENO))
		ft_putstr_fd("exit\n", 1);
	if (argv[1])
	{
		if (!validate_numeric(argv[1]))
		{
			ft_putstr_fd("minishell: exit: numeric argument required\n", 2);
			sh->last_status = 2;
			sh->running = 0;
			return (2);
		}
		if (argv[2])
		{
			ft_putstr_fd("minishell: exit: too many arguments\n", 2);
			sh->last_status = 1;
			return (1);
		}
		sh->last_status = compute_exit_status(argv[1]);
	}
	sh->running = 0;
	return (sh->last_status);
}

/**
 * @brief check if arg is a valid numeric exit argument.
 *
 * @param arg The argument string to validate (may start with + or -)
 * @return 1 if valid numeric, 0 otherwise
 */
static int	validate_numeric(char *arg)
{
	char	*p;

	p = arg;
	if (*p == '+' || *p == '-')
		p++;
	if (*p == '\0')
		return (0);
	while (*p)
	{
		if (!ft_isdigit(*p))
			return (0);
		p++;
	}
	return (1);
}

/**
 * @brief Compute exit status from validated numeric string (0-255).
 *
 * Parses the numeric argument and reduces it modulo 256 to match shell
 * semantics. Negative values are converted to the corresponding positive
 * exit code in the 0..255 range.
 *
 * @param arg The validated numeric string to convert
 * @return int Exit code in range 0..255
 */
static int	compute_exit_status(char *arg)
{
	long long	val;
	int			status;

	val = ft_strtoll(arg, NULL, 10);
	status = (int)(val % 256);
	if (status < 0)
		status += 256;
	return (status);
}
