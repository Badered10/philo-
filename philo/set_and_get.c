/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_and_get.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baouragh <baouragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 13:46:41 by baouragh          #+#    #+#             */
/*   Updated: 2024/09/19 15:10:39 by baouragh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	get_bool(pthread_mutex_t *mutex, bool *die_flag)
{
	bool	res;

	res = 0;
	pthread_mutex_lock(mutex);
	res = *die_flag;
	pthread_mutex_unlock(mutex);
	return (res);
}

void	set_bool(pthread_mutex_t *mutex, bool *die_flag, bool val)
{
	pthread_mutex_lock(mutex);
	*die_flag = val;
	pthread_mutex_unlock(mutex);
}

int	get_state(pthread_mutex_t *mutex, t_state *state)
{
	int	res;

	res = -1;
	pthread_mutex_lock(mutex);
	res = *state;
	pthread_mutex_unlock(mutex);
	return (res);
}

void	set_state(pthread_mutex_t *mutex, t_state *state, int new)
{
	pthread_mutex_lock(mutex);
	*state = new;
	pthread_mutex_unlock(mutex);
}

void	ft_usleep(time_t time, t_data *data)
{
	time_t	start;

	start = get_t();
	while (1)
	{
		if (get_t() - start >= time || get_bool(data->philos.p_semaphore, &data->die_flag))
			break ;
		usleep(500);
	}
}
