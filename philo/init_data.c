/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baouragh <baouragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 13:41:14 by baouragh          #+#    #+#             */
/*   Updated: 2024/09/16 20:27:39 by baouragh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_philo	*create_philos(t_data *data)
{
	t_philo	*philos;
	int		x;

	x = 0;
	philos = malloc(sizeof(t_philo) * data->num_of_philos);
	if (!philos)
		return (NULL);
	memset(philos, 0, sizeof(t_philo) * data->num_of_philos);
	while (x < data->num_of_philos)
	{
		philos[x].state = NOTYET;
		philos[x].id = x + 1;
		philos[x].left_fork = &data->forks[x];
		if (x + 1 == 1)
			philos[x].right_fork = &data->forks[data->num_of_philos - 1];
		else
			philos[x].right_fork = &data->forks[x - 1];
		philos[x].data = data;
		if (philo_init_mutex(philos, x))
			return (NULL);
		x++;
	}
	return (philos);
}

pthread_mutex_t	*create_forks(long np)
{
	pthread_mutex_t	*forks;
	long			i;

	i = 0;
	forks = malloc(sizeof(pthread_mutex_t) * np);
	if (!forks)
		return (NULL);
	while (i < np)
	{
		if (pthread_mutex_init(&forks[i], NULL))
		{
			while (i > 0)
			{
				pthread_mutex_destroy(&forks[i]);
				i--;
			}
			free(forks);
			return (printf("Failed to create a mutex!\n"), NULL);
		}
		i++;
	}
	return (forks);
}

void	destroy_mutexs(t_data *data)
{
	pthread_mutex_destroy(&data->scan);
	free(data);
}

int	init_data_mutexs(t_data *data)
{
	if (pthread_mutex_init(&data->scan, NULL))
		return (printf("Faild to init a mutex\n"), 1);
	return (0);
}

t_data	*set_data(int argc, char **argv)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (!data)
		return (NULL);
	data->num_of_philos = ft_atol(argv[1]);
	data->ttd = ft_atol(argv[2]);
	data->tte = ft_atol(argv[3]);
	data->tts = ft_atol(argv[4]);
	data->num_of_meals = -10;
	if (init_data_mutexs(data))
		return (destroy_mutexs(data), NULL);
	if (argc == 6)
		data->num_of_meals = ft_atol(argv[5]);
	if (data->num_of_meals == -1 || data->num_of_philos == -1 || data->ttd == -1
		|| data->tte == -1 || data->tts == -1)
		return (destroy_mutexs(data), NULL);
	data->forks = create_forks(data->num_of_philos);
	if (!data->forks)
		return (destroy_mutexs(data), NULL);
	data->philos = create_philos(data);
	if (!data->philos)
		return (destroy_forks(data), destroy_mutexs(data), NULL);
	return (data);
}
