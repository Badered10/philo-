/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scaner_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baouragh <baouragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 13:49:06 by baouragh          #+#    #+#             */
/*   Updated: 2024/09/16 15:09:42 by baouragh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	get_curr_diff(t_data *data, time_t *curr, time_t *diff)
{
	*curr = get_t() - data->start;
	sem_wait(&data->philos.p_semaphore);
	*diff = *curr - data->philos.last_meal_time;
	sem_post(&data->philos.p_semaphore);
}

void	*scan_death(void *infos)
{
	t_philo	*philo;
	time_t	curr;
	time_t	diff;
	int		i;

	i = 0;
	philo = infos;
	while (1)
	{
		get_curr_diff(philo->data, &curr, &diff);
		if (diff > philo->data->ttd)
		{
			printf("%ld %ld died\n", curr, philo->id);
			exit (1);
		}
		if (get_state(&philo->p_semaphore, &philo->state) == DONE)
			break ;
		i++;
		if (i == philo->data->num_of_philos)
			i = 0;
	}
	return (NULL);
}