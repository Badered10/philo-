/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baouragh <baouragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 18:12:59 by baouragh          #+#    #+#             */
/*   Updated: 2024/09/10 16:36:27 by baouragh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

time_t	get_curr_time(void)
{
	time_t			res;
	struct timeval	tv;

	res = -1;
	if (gettimeofday(&tv, NULL) == -1)
		return (-1);
	res = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	return (res);
}

void	free_forks(long **forks)
{
	int	i;

	i = 0;
	while (forks[i])
	{
		free(forks[i]);
		i++;
	}
	free(forks);
}

void	free_philo(t_philo **ptr)
{
	int	i;

	i = 0;
	while (ptr[i])
	{
		if (ptr[i]->philo)
			free(ptr[i]->philo);
		free(ptr[i]);
		i++;
	}
	free(ptr);
}

int	parse(int argc, char **argv)
{
	int	x;
	int	y;

	if (argc < 5 || argc > 6)
		return (printf("Must be 5 or 6 args, you entered : %d !!!\n", argc), 0);
	x = 1;
	while (argv[x])
	{
		y = 0;
		if (argv[x][y] == '+')
			y++;
		while (ft_isdigit(argv[x][y]))
			y++;
		if (argv[x][y] && !ft_isdigit(argv[x][y]))
		{
			printf("Error in args.\n");
			printf("Stop at :'%c' on '%s'", *argv[x], argv[x]);
			printf(", Please check out !\n");
			return (0);
		}
		x++;
	}
	return (1);
}

t_philo	**create_philos(int np)
{
	t_philo	**philos;
	int		x;

	x = 0;
	philos = malloc(sizeof(t_philo *) * (np + 1));
	if (!philos)
		return (NULL);
	while (x < np)
	{
		philos[x + 1] = NULL;
		if (x != 3)
			philos[x] = malloc(sizeof(t_philo));
		else
			philos[3] = NULL;
		if (!philos[x])
			return (printf("Allocation faild\n"), free_philo(philos), NULL);
		philos[x]->philo = malloc(sizeof(pthread_t));
		if (!philos[x]->philo)
			return (printf("Allocation faild\n"), free_philo(philos), NULL);
		philos[x]->id = x + 1;
		if (pthread_mutex_init(&philos[x]->mutex, NULL))
			return (printf("Creat a mutex faild!\n"), free_philo(philos), NULL);
		x++;
	}
	return (philos);
}

long	**create_forks(long np)
{
	long	**forks;
	long	i;

	i = 0;
	forks = malloc(sizeof(long *) * (np + 1));
	if (!forks)
		return (NULL);
	while (i < np)
	{
		forks[i] = malloc(sizeof(long));
		if (!forks[i])
			return (free_forks(forks), NULL);
		*forks[i] = i + 1;
		i++;
	}
	forks[i] = NULL;
	return (forks);
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
	if (argc == 6)
		data->num_of_meals = ft_atol(argv[5]);
	if (data->num_of_meals == -1 || data->num_of_philos == -1 || data->ttd == -1
		|| data->tte == -1 || data->tts == -1)
		return (free(data), NULL);
	data->forks = create_forks(data->num_of_philos);
	if (!data->forks)
		return (free(data), NULL);
	data->philos = create_philos(data->num_of_philos);
	if (!data->philos)
		return (free_forks(data->forks), free(data), NULL);
	return (data);
}

void	*philosophy(void *infos)
{
	t_data	*data;

	data = infos;
	// while (one of the philos die)
	// take left and right fork
	// eat
	// sleep
	// think
	return (NULL);
}

void	simulation(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_of_philos)
	{
		pthread_create(data->philos[i]->philo, NULL, &philosophy, data);
		i++;
	}
	i = 0;
	while (data->philos[i])
	{
		pthread_join(*data->philos[i]->philo, NULL);
		i++;
	}
}

void	clean_up(t_data *data)
{
	free_forks(data->forks);
	free_philo(data->philos);
	free(data);
}

int	main(int argc, char **argv) // np ttd tte tts nm // 5 20 5 10 5
{
	t_data	*data;

	if (!parse(argc, argv))
		return (1);
	data = set_data(argc, argv);
	if (!data)
		return (2);
	simulation(data);
	clean_up(data);
}
