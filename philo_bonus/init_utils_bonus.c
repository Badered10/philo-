/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_utils_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baouragh <baouragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 13:43:30 by baouragh          #+#    #+#             */
/*   Updated: 2024/09/22 19:13:03 by baouragh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	unlink_semaphores(t_data *data)
{
	if (data->died)
		sem_unlink(data->died->name);
	if (data->forks)
		sem_unlink(data->forks->name);
	if (data->sh_value)
		sem_unlink(data->sh_value->name);

	if(data->philos.full)
		sem_unlink(data->philos.full->name);
	if(data->philos.meal)
		sem_unlink(data->philos.meal->name);
	if(data->philos.value)
		sem_unlink(data->philos.value->name);
		
}

void	clean_up(t_data *data, unsigned int exit_num)
{
	unlink_semaphores(data);
	free_garbage(data->list);
	free(data);
	exit (exit_num);
}

time_t	get_t(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
		return (-1);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}
