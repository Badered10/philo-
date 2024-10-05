/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scaner_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baouragh <baouragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 13:49:06 by baouragh          #+#    #+#             */
/*   Updated: 2024/10/05 14:22:11 by baouragh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	get_curr_diff(t_philo *philo, time_t *curr, time_t *diff)
{
	time_t	last;

	last = get_long(philo->meal->sem, &philo->last_meal_time);
	*curr = get_t() - philo->start;
	*diff = *curr - last;
}

void	*scan_death(void *infos)
{
	t_philo	*philo;
	time_t	diff;
	time_t	curr;

	philo = infos;
	while (!get_bool(philo->full->sem, &philo->full_flag))
	{
		get_curr_diff(philo, &curr, &diff);
		if (diff > philo->data->ttd)
		{
			sem_wait(philo->data->sh_value->sem);
			set_bool(philo->value->sem, &philo->die_flag, 1);
			sem_wait(philo->value->sem);
			if (!get_bool(philo->full->sem, &philo->full_flag))
				announcement(philo, curr);
			sem_post(philo->value->sem);
			sem_post(philo->data->sh_value->sem);
			break ;
		}
		usleep(8000);
	}
	return (NULL);
}

long	get_value(sem_t *from, sem_t *garde)
{
	long	value;

	value = 0;
	sem_wait(garde);
	value = from->__align;
	sem_post(garde);
	return (value);
}

long	ft_get_value(sem_t *from)
{
	long	value;

	value = from->__align;
	return (value);
}

void	*check_wait(void *data)
{
	t_wait	*wait;
	int		x;

	x = 0;
	wait = data;
	while (get_bool(wait->stop, &wait->stop_flag))
	{
		if (!ft_get_value(wait->died) || ft_get_value(wait->died) > 1)
		{
			usleep(2000);
			while (x < wait->pids_num)
			{
				kill(wait->pids[x], SIGKILL);
				x++;
			}
			break ;
		}
	}
	return (NULL);
}
