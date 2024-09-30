/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_and_get_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baouragh <baouragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 13:46:41 by baouragh          #+#    #+#             */
/*   Updated: 2024/09/29 10:45:46 by baouragh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

bool	get_bool(sem_t *semaphore, bool *varible)
{
	bool	res;

	res = 0;
	sem_wait(semaphore);
	res = *varible;
	sem_post(semaphore);
	return (res);
}

void	set_bool(sem_t *semaphore, bool *varible, bool val)
{
	sem_wait(semaphore);
	*varible = val;
	sem_post(semaphore);
}

long	get_long(sem_t *semaphore, long *varible)
{
	long	res;

	res = 0;
	sem_wait(semaphore);
	res = *varible;
	sem_post(semaphore);
	return (res);
}

void	set_long(sem_t *semaphore, long *varible, long val)
{
	sem_wait(semaphore);
	*varible = val;
	sem_post(semaphore);
}

void	ft_usleep(time_t time, t_data *data)
{
	time_t	start;

	start = get_t();
	while (1)
	{
		if (get_t() - start >= time || !get_value(data->died->sem,
				data->sh_value->sem))
			break ;
		usleep(500);
	}
}
