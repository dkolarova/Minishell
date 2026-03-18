/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_cleanup.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjusta <mjusta@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 19:34:58 by mjusta            #+#    #+#             */
/*   Updated: 2025/11/21 16:42:01 by mjusta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_CLEANUP_H
# define LIBFT_CLEANUP_H

# include <stdlib.h>

void	free_char_arr(char **arr);
void	free_int_arr(int **arr, int len);

#endif