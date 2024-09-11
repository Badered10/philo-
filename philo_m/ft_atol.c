/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baouragh <baouragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 17:53:19 by baouragh          #+#    #+#             */
/*   Updated: 2024/09/11 03:06:32 by baouragh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	ft_atol(const char *str)
{
	char	*var;
	long	res;
	long	digit;

	var = (char *)str;
	res = 0;
	while ((*var >= 9 && *var <= 13) || *var == 32)
		var++;
	if (*var == '-' || *var == '+')
	{
		if (*var == '-')
			return (-1);
		var++;
	}
	while (*var && *var >= '0' && *var <= '9')
	{
		digit = *var - '0';
		if (res > (LONG_MAX - digit) / 10)
			return (printf("Invalid arg '%s'%s\n", str, ERR_MSG), -1);
		res = res * 10 + digit;
		var++;
	}
	return (res);
}
