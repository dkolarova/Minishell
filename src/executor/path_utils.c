/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjusta <mjusta@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 08:47:21 by dkolarov          #+#    #+#             */
/*   Updated: 2025/12/30 00:46:15 by mjusta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static char	*find_command_in_path(char *cmd, char **path_split);
static void	free_path_array(char **path_split);

/**
 * @brief Resolve the full path of a command.
 *
 * If `cmd` contains a '/', it is returned (if accessible). Otherwise the
 * function searches the PATH environment variable and returns the first
 * matching executable path. Caller must free the returned string.
 *
 * @param cmd Command name or path
 * @param path_dirs Optional pre-split PATH directories (currently unused)
 * @return char* Newly allocated path string on success, NULL on failure
 */
char	*resolve_command_path(char *cmd, t_env *env)
{
	char	*path_env;
	char	**path_split;
	char	*result;

	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, F_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	if (env)
		path_env = env_get(env, "PATH");
	else
		path_env = getenv("PATH");
	if (!path_env)
		return (NULL);
	path_split = ft_split(path_env, ':');
	if (!path_split)
		return (NULL);
	result = find_command_in_path(cmd, path_split);
	free_path_array(path_split);
	return (result);
}

/**
 * @brief Search the provided PATH directories for an executable.
 *
 * Concatenates each entry in `path_split` with `cmd` and checks accessibility.
 * Returns the first matching path or NULL if none found. Caller must free the
 * returned string.
 *
 * @param cmd Command name to search for
 * @param path_split NULL-terminated array of PATH directories
 * @return char* Newly allocated path string on success, NULL otherwise
 */
static char	*find_command_in_path(char *cmd, char **path_split)
{
	char	*full_path;
	char	*temp_path;
	int		i;

	i = 0;
	while (path_split[i])
	{
		temp_path = ft_strjoin(path_split[i], "/");
		if (!temp_path)
			return (NULL);
		full_path = ft_strjoin(temp_path, cmd);
		free(temp_path);
		if (!full_path)
			return (NULL);
		if (access(full_path, F_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

/**
 * @brief Free an array of strings created by ft_split.
 *
 * Frees each element and the array itself. Designed for PATH split arrays.
 *
 * @param path_split NULL-terminated array to free
 * @return void
 */
static void	free_path_array(char **path_split)
{
	int	i;

	i = 0;
	while (path_split[i])
		free(path_split[i++]);
	free(path_split);
}
