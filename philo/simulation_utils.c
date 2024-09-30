/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baouragh <baouragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 13:48:16 by baouragh          #+#    #+#             */
/*   Updated: 2024/09/30 14:52:31 by baouragh         ###   ########.fr       */
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
	time = get_t() - philo->start;
	if (!get_bool(&philo->data->scan, &philo->data->die_flag))
	{
		pthread_mutex_lock(&philo->data->print);
		printf("%ld %ld has taken a fork\n", time, philo->id);
		pthread_mutex_unlock(&philo->data->print);
	}
}

void	take_right_fork(t_philo *philo)
{
	time_t	time;

	pthread_mutex_lock(philo->right_fork);
	time = get_t() - philo->start;
	if (!get_bool(&philo->data->scan, &philo->data->die_flag))
	{
		pthread_mutex_lock(&philo->data->print);
		printf("%ld %ld has taken a fork\n", time, philo->id);
		pthread_mutex_unlock(&philo->data->print);
	}
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

int	eating(t_philo *philo)
{
	time_t	time;

	if (take_forks(philo))
	{
		if (philo->data->num_of_philos == 1)
			pthread_mutex_unlock(philo->left_fork);
		return (-1);
	}
	time = get_t() - philo->start;
	pthread_mutex_lock(&philo->meal_m);
	philo->last_meal_time = time;
	pthread_mutex_unlock(&philo->meal_m);
	if (get_bool(&philo->data->scan, &philo->data->die_flag))
		return (put_forks(philo), -1);
	pthread_mutex_lock(&philo->data->print);
	printf("%ld %ld is eating\n", time, philo->id);
	pthread_mutex_unlock(&philo->data->print);
	philo->eaten_meals++;
	if (philo->eaten_meals == philo->data->num_of_meals)
		philo->full = 1;
	ft_usleep(philo->data->tte, philo->data);
	put_forks(philo);
	return (0);
}
