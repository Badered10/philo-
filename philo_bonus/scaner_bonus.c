/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scaner_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baouragh <baouragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 13:49:06 by baouragh          #+#    #+#             */
/*   Updated: 2024/09/29 12:16:23 by baouragh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	get_curr_diff(t_philo *philo , time_t *curr , time_t *diff)
{
	time_t last;

	last = get_long(philo->meal->sem, &philo->last_meal_time);
	*curr = get_t() - philo->start;
	*diff = *curr - last;
}

void	*scan_death(void *infos)
{
	t_philo *philo;
	time_t	diff;
	time_t	curr;

    philo = infos;
    while(!get_bool(philo->full->sem, &philo->full_flag))
    { 
		get_curr_diff(philo , &curr , &diff);
        if (diff > philo->data->ttd)
        {
            sem_wait(philo->data->sh_value->sem);
            printf("%ld %ld is dead\n",curr, philo->id);
            set_bool(philo->value->sem, &philo->die_flag, 1);
			sem_wait(philo->data->died->sem);
            sem_post(philo->data->sh_value->sem);
            break;
        }
        if (!get_value(philo->full->sem, philo->value->sem))
            break;
        usleep(500);
    }
    return (NULL);
}
