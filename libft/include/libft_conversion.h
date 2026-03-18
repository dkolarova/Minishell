/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_conversion.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjusta <mjusta@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 02:07:49 by mjusta            #+#    #+#             */
/*   Updated: 2025/11/21 16:45:49 by mjusta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_CONVERSION_H
# define LIBFT_CONVERSION_H

int			ft_atoi(const char *nptr);
long long	ft_strtoll(const char *nptr, char **endptr, int base);
char		*ft_itoa(int n);
int			ft_tolower(int c);
int			ft_toupper(int c);

#endif