/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjusta <mjusta@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 09:35:57 by dkolarov          #+#    #+#             */
/*   Updated: 2025/11/21 16:30:32 by mjusta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtins.h"
#include "env.h"
#include "stdio.h"

static char	*get_cd_target(char **argv);

/**
 * @brief Implement cd builtin - change directory
 * 
 * @param argv Command arguments
 * @param env Environment variables
 * @return int Exit status
 */
int	bi_cd(char **argv, t_env **env)
{
	char	cwd[PATH_MAX_LEN];
	char	*target;
	char	*oldpwd;

	oldpwd = getcwd(cwd, sizeof(cwd));
	target = get_cd_target(argv);
	if (!target)
	{
		ft_putstr_fd("minishell: cd: HOME not set\n", 2);
		return (1);
	}
	if (chdir(target) != 0)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		perror(target);
		return (1);
	}
	if (oldpwd)
		set_env(env, "OLDPWD", oldpwd);
	if (getcwd(cwd, sizeof(cwd)))
		set_env(env, "PWD", cwd);
	if (argv[1] && ft_strncmp(argv[1], "-", 2) == 0)
		ft_putendl_fd(cwd, 1);
	return (0);
}

static char	*get_cd_target(char **argv)
{
	if (!argv[1] || ft_strncmp(argv[1], "~", 2) == 0)
		return (getenv("HOME"));
	return (argv[1]);
}
