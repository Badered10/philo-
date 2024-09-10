/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baouragh <baouragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 18:12:59 by baouragh          #+#    #+#             */
/*   Updated: 2024/09/10 18:32:30 by baouragh         ###   ########.fr       */
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

void	free_forks(t_fork **forks)
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

t_philo	**create_philos(int np, t_data *data)
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
		philos[x] = malloc(sizeof(t_philo));
		if (!philos[x])
			return (printf(ALLOC_ERR), free_philo(philos), NULL);
		philos[x]->philo = malloc(sizeof(pthread_t));
		if (!philos[x]->philo)
			return (printf(ALLOC_ERR), free_philo(philos), NULL);
		if (pthread_mutex_init(&philos[x]->mutex, NULL))
			return (printf("Creat a mutex faild!\n"), free_philo(philos), NULL);
		philos[x]->id = x + 1;
		philos[x]->data = data;
		x++;
	}
	return (philos);
}

t_fork	**create_forks(long np)
{
	t_fork	**forks;
	long	i;

	i = 0;
	forks = malloc(sizeof(t_fork *) * (np + 1));
	if (!forks)
		return (NULL);
	while (i < np)
	{
		forks[i] = malloc(sizeof(t_fork));
		if (!forks[i])
			return (free_forks(forks), NULL);
		forks[i]->id = i + 1;
		forks[i]->state = AV;
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
	data->philos = create_philos(data->num_of_philos, data);
	if (!data->philos)
		return (free_forks(data->forks), free(data), NULL);
	return (data);
}

void	take_left_fork(t_philo *philo)
{
	time_t time;

	pthread_mutex_lock(&philo->mutex);
	time = get_curr_time() - philo->data->start;
	if (philo->data->die_flag)
	{
		philo->data->forks[philo->id - 1]->state = NOT_AV;
		printf("At :%ld\tPhilo %ld, has Take left fork :%ld\n",time, philo->id, philo->id);
	}
	pthread_mutex_unlock(&philo->mutex);
}

void	take_right_fork(t_philo *philo, long id)
{
	time_t time;

	pthread_mutex_lock(&philo->mutex);
	time = get_curr_time() - philo->data->start;
	if (philo->data->die_flag)
	{
		philo->data->forks[id]->state = NOT_AV;
		printf("At :%ld\tPhilo %ld, has Take right fork :%ld\n",time, philo->id, id);
	}
	pthread_mutex_unlock(&philo->mutex);
}

void	eating(t_philo *philo, long right_id)
{
	time_t time;

	pthread_mutex_lock(&philo->mutex);
	time = get_curr_time() - philo->data->start;
	if (philo->data->die_flag)
	{
		if (philo->last_meal_time)
		philo->last_meal_time = time;
		printf("At :%ld\tPhilo %ld, Eating\n",time, philo->id);
	}
	pthread_mutex_unlock(&philo->mutex);
	pthread_mutex_lock(&philo->mutex);
	philo->data->forks[philo->id - 1]->state = AV;
	philo->data->forks[right_id]->state = AV;
	pthread_mutex_unlock(&philo->mutex);
	usleep(philo->data->tts);
}

void	*philosophy(void *infos)
{
	t_philo	*philo;
	long right_id;

	philo = infos;
	if (philo->id == 1)
		right_id = philo->data->num_of_philos -1;
	else
		right_id = philo->id - 2;
	// else if (philo->id == 1)
	// 	right_id = philo->data->num_of_philos - 1;
	if (philo->id % 3 == 0)
		usleep(100);
	while (philo->data->die_flag)
	{
		while (philo->data->forks[philo->id - 1]->state == NOT_AV)
			;
		take_left_fork(philo);
		while (philo->data->forks[right_id]->state == NOT_AV)
			;
		take_right_fork(philo, right_id);
		eating(philo, right_id);
		printf("At :%ld\tPhilo %ld, Sleeping\n",get_curr_time() - philo->data->start, philo->id);
		usleep(philo->data->tts);
		printf("At :%ld\tPhilo %ld, Thinking\n",get_curr_time() - philo->data->start, philo->id);
	}
	// take left and right forks if they are available
	// eat
	// retrun left and right forks
	// sleep
	// think
	return (NULL);
}

void	simulation(t_data *data)
{
	int	i;

	i = 0;
	data->start = get_curr_time();
	data->die_flag = 1;
	while (i < data->num_of_philos)
	{
		pthread_create(data->philos[i]->philo, NULL, &philosophy, data->philos[i]);
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
