/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_env_and_path.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjusta <mjusta@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 17:15:39 by dkolarov          #+#    #+#             */
/*   Updated: 2025/12/30 00:45:18 by mjusta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

/**
 * @brief Prepare envp and resolve command path.
 *
 * Builds the environment array and resolves the command's executable path.
 * Exits on environment allocation failure or when no command is provided.
 * The resulting envp and resolved (or NULL) path are stored in the outputs.
 *
 * @param cmd Command node containing argv
 * @param sh Shell state (environment, resources)
 * @param envp_out Output pointer to the generated envp array
 * @param path_out Output pointer to the resolved command path (or NULL)
 */
void	prepare_env_and_path(t_cmd *cmd, t_shell *sh,
				char ***envp_out, char **path_out)
{
	*envp_out = env_to_environ(sh->env);
	if (!*envp_out)
	{
		cleanup_shell(sh);
		exit(1);
	}
	*path_out = NULL;
	if (!cmd->argv[0])
		exit(0);
	if (ft_strchr(cmd->argv[0], '/'))
		*path_out = ft_strdup(cmd->argv[0]);
	else
		*path_out = resolve_command_path(cmd->argv[0], sh->env);
}
