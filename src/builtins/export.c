/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkolarov <dkolarov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 10:39:02 by dkolarov          #+#    #+#             */
/*   Updated: 2025/12/08 10:45:48 by dkolarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtins.h"
#include "env.h"

/**
 * @brief Implement export builtin
 * 
 * @param argv Command arguments
 * @param env Environment variables
 * @return int Exit status
 */
int	bi_export(char **argv, t_env **env)
{
	int	i;

	if (!argv[1])
	{
		print_env_export(*env);
		return (0);
	}
	i = 1;
	while (argv[i])
	{
		export_process_arg(argv[i], env);
		i++;
	}
	return (0);
}

void	export_process_arg(char *arg, t_env **env)
{
	if (!is_valid_identifier(arg))
	{
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		return ;
	}
	export_set_or_mark(arg, env);
}

void	export_set_or_mark(char *arg, t_env **env)
{
	char	*eq;
	char	*key;
	char	*val;

	eq = ft_strchr(arg, '=');
	if (eq)
	{
		key = ft_substr(arg, 0, eq - arg);
		val = ft_strdup(eq + 1);
		env_set(env, key, val, 1);
		free(key);
		free(val);
	}
	else
		env_set(env, arg, NULL, 1);
}

int	is_valid_identifier(const char *s)
{
	int	i;

	if (!s || (!ft_isalpha(s[0]) && s[0] != '_'))
		return (0);
	i = 1;
	while (s[i] && s[i] != '=')
	{
		if (!ft_isalnum(s[i]) && s[i] != '_')
			return (0);
		i++;
	}
	return (1);
}
