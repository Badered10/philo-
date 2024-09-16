/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_utils_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baouragh <baouragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 13:48:16 by baouragh          #+#    #+#             */
/*   Updated: 2024/09/16 15:00:33 by baouragh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	put_forks(t_philo *philo)
{
	sem_post(&philo->data->ps_semaphore);
	sem_post(&philo->data->ps_semaphore);
}

void	take_fork(t_philo *philo)
{
	time_t	time;

	sem_wait(&philo->data->ps_semaphore);
	time = get_t() - philo->data->start;
	if (!get_bool(&philo->p_semaphore, &philo->data->die_flag))
		printf("%ld %ld has taken a fork\n", time, philo->id);
}

int	take_forks(t_philo *philo)
{
	if (get_bool(&philo->p_semaphore, &philo->data->die_flag))
		return (-1);
	if (philo->data->num_of_philos != 1)
	{
		take_fork(philo);
		take_fork(philo);
		return (0);
	}
	else
	{
		take_fork(philo);
		while ((!get_bool(&philo->p_semaphore, &philo->data->die_flag)))
			;
		return (-1);
	}
}
