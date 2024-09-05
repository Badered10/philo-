/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baouragh <baouragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 17:53:19 by baouragh          #+#    #+#             */
/*   Updated: 2024/09/05 16:08:29 by baouragh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	ft_atol(const char *str)
{
	char	*var;
	size_t	res;
	size_t	old;

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
		old = res;
		res = res * 10 + (*var - '0');
		if(res > LONG_MAX || old > res)
			return (printf("Invalid arg '%s'%s\n",str, ERR_MSG), -1);
		else
			printf("RES : %zu\n",res);
		var++;
	}
	return ((long)res);
}
