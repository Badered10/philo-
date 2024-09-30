/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_utils_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baouragh <baouragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 13:43:30 by baouragh          #+#    #+#             */
/*   Updated: 2024/09/29 10:36:48 by baouragh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	close_unlink(t_nsem *sem, bool mode)
{
	if (!sem || !sem->sem || !sem->name)
	{
		if (sem)
			free(sem);
		return ;
	}
	sem_close(sem->sem);
	if (mode)
		sem_unlink(sem->name);
	free(sem->name);
	free(sem);
}

void	unlink_semaphores(t_data *data, bool mode)
{
	close_unlink(data->died, mode);
	close_unlink(data->forks, mode);
	close_unlink(data->sh_value, mode);
	close_unlink(data->stop, mode);
	close_unlink(data->philos.full, mode);
	close_unlink(data->philos.meal, mode);
	close_unlink(data->philos.value, mode);
}

void	clean_up(t_data *data, unsigned int exit_num, bool mode)
{
	unlink_semaphores(data, mode);
	free(data->pids);
	free(data);
	exit(exit_num);
}

time_t	get_t(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
		return (-1);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}
