/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baouragh <baouragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 13:43:30 by baouragh          #+#    #+#             */
/*   Updated: 2024/09/17 13:54:45 by baouragh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	destroy_forks(t_data *data)
{
	int	i;

	i = data->num_of_philos - 1;
	while (i >= 0)
	{
		pthread_mutex_destroy(&data->forks[i]);
		i--;
	}
	free(data->forks);
}

void	free_philos(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_of_philos)
	{
		pthread_mutex_destroy(&data->philos[i].meal_m);
		i++;
	}
	free(data->philos);
}

int	philo_init_mutex(t_philo *philos, int x)
{
	if (pthread_mutex_init(&philos[x].state_m, NULL))
	{
		while (x > 0)
		{
			pthread_mutex_destroy(&philos[x].state_m);
			x--;
		}
		free(philos);
		return (printf("Failed to create a mutex philos !\n"), -1);
	}
	if (pthread_mutex_init(&philos[x].meal_m, NULL))
	{
		while (x > 0)
		{
			pthread_mutex_destroy(&philos[x].meal_m);
			x--;
		}
		free(philos);
		return (printf("Failed to create a mutex philos !\n"), -1);
	}
	return (0);
}

void	clean_up(t_data *data)
{
	destroy_forks(data);
	free_philos(data);
	free(data);
}

time_t	get_t(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
		return (-1);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}
