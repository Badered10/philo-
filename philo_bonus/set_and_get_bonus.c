/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_and_get_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baouragh <baouragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 13:46:41 by baouragh          #+#    #+#             */
/*   Updated: 2024/09/22 22:42:52 by baouragh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

bool	get_bool(sem_t *semaphore, bool *die_flag)
{
	bool	res;

	res = 0;
	sem_wait(semaphore);
	res = *die_flag;
	sem_post(semaphore);
	return (res);
}

void	set_bool(sem_t *semaphore, bool *die_flag, bool val)
{
	sem_wait(semaphore);
	*die_flag = val;
	sem_post(semaphore);
}

int	get_state(sem_t *semaphore, t_state *state)
{
	int	res;

	res = -1;
	sem_wait(semaphore);
	res = *state;
	sem_post(semaphore);
	return (res);
}

void	set_state(sem_t *semaphore, t_state *state, int new)
{
	sem_wait(semaphore);
	*state = new;
	sem_post(semaphore);
}


void	ft_usleep(time_t time, t_data *data)
{
	(void)data;
	time_t	start;

	printf("ENTER FT_USLEEP \n");
	start = get_t();
	while (1)
	{
		// if (get_t() - start >= time || get_bool(data->philos.value->sem, &data->philos.die_flag))
		if (get_t() - start >= time)
		{
				printf("OUT FT_USLEEP \n");
			break ;
		}
		usleep(500);
	}
}
