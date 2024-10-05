/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_utils_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baouragh <baouragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 13:48:16 by baouragh          #+#    #+#             */
/*   Updated: 2024/10/02 18:16:44 by baouragh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	put_forks(t_philo *philo)
{
	sem_post(philo->data->forks->sem);
	sem_post(philo->data->forks->sem);
}

void	take_fork(t_philo *philo)
{
	sem_wait(philo->data->forks->sem);
	if (get_value(philo->data->died->sem, philo->data->sh_value->sem))
	{
		sem_wait(philo->value->sem);
		if (get_value(philo->data->died->sem, philo->data->sh_value->sem))
			printf("%ld %ld has taken a fork\n", get_t() - philo->start,
				philo->id);
		sem_post(philo->value->sem);
	}
}

int	take_forks(t_philo *philo)
{
	if (!get_value(philo->data->died->sem, philo->data->sh_value->sem))
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
		while (!get_bool(philo->value->sem, &philo->die_flag))
			usleep(500);
		return (-1);
	}
}

void	sleep_and_think(t_philo *philo)
{
	if (get_value(philo->data->died->sem, philo->data->sh_value->sem))
	{
		sem_wait(philo->value->sem);
		printf("%ld %ld is sleeping\n", get_t() - philo->start, philo->id);
		sem_post(philo->value->sem);
	}
	ft_usleep(philo->data->tts, philo->data);
	if (get_value(philo->data->died->sem, philo->data->sh_value->sem))
	{
		sem_wait(philo->value->sem);
		printf("%ld %ld is thinking\n", get_t() - philo->start, philo->id);
		sem_post(philo->value->sem);
	}
	if (philo->data->num_of_philos % 2)
		usleep(1000);
}
