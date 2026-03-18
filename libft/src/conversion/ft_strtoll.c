/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtoll.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkolarov <dkolarov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 10:15:57 by dkolarov          #+#    #+#             */
/*   Updated: 2025/11/10 10:41:42 by dkolarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_conversion.h"
#include "libft_char.h"

long long	ft_strtoll(const char *nptr, char **endptr, int base)
{
	const char	*s;
	long long	acc;
	int			sign;

	s = nptr;
	acc = 0;
	sign = 1;
	if (!s)
		return (0);
	while (*s == ' ' || (*s >= '\t' && *s <= '\r'))
		s++;
	if (*s == '+' || *s == '-')
	{
		if (*s == '-')
			sign = -1;
		s++;
	}
	if (base == 0)
		base = 10;
	while (*s && ft_isdigit(*s))
		acc = acc * base + (*s++ - '0');
	if (endptr)
		*endptr = (char *)s;
	return (acc * sign);
}
