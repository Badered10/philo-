/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baouragh <baouragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 13:48:16 by baouragh          #+#    #+#             */
/*   Updated: 2024/09/15 13:48:26 by baouragh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	put_forks(t_philo *philo)
{
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
}

void	take_left_fork(t_philo *philo)
{
	time_t	time;

	pthread_mutex_lock(philo->left_fork);
	time = get_t() - philo->data->start;
	if (!get_bool(&philo->data->scan, &philo->data->die_flag))
		printf("%ld %ld has taken a fork\n", time, philo->id);
}

void	take_right_fork(t_philo *philo)
{
	time_t	time;

	pthread_mutex_lock(philo->right_fork);
	time = get_t() - philo->data->start;
	if (!get_bool(&philo->data->scan, &philo->data->die_flag))
		printf("%ld %ld has taken a fork\n", time, philo->id);
}

int	take_forks(t_philo *philo)
{
	if (get_bool(&philo->data->scan, &philo->data->die_flag))
		return (-1);
	if (philo->data->num_of_philos != 1)
	{
		if (philo->id % 2 == 1)
		{
			take_right_fork(philo);
			take_left_fork(philo);
		}
		else
		{
			take_left_fork(philo);
			take_right_fork(philo);
		}
		return (0);
	}
	else
	{
		take_left_fork(philo);
		while (!get_bool(&philo->data->scan, &philo->data->die_flag))
			;
		return (-1);
	}
}
