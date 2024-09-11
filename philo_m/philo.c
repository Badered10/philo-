/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baouragh <baouragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 18:12:59 by baouragh          #+#    #+#             */
/*   Updated: 2024/09/11 02:50:26 by baouragh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <string.h>

void	destroy_forks(t_data *data)
{
	int i;

	i = data->num_of_philos - 1;
	while (i >= 0)
	{
		pthread_mutex_destroy(&data->forks[i]);
		i--;
	}
	free(data->forks);
}

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
		philos[x].id = x + 1;
		philos[x].left_fork = &data->forks[x];
		if (x + 1 != data->num_of_philos )
			philos[x].right_fork = &data->forks[x + 1];
		else
			philos[x].right_fork = &data->forks[0];
		philos[x].data = data;
		x++;
	}
	return (philos);
}

pthread_mutex_t	*create_forks(long np)
{
	pthread_mutex_t	*forks;
	long	i;

	i = 0;
	forks = malloc(sizeof(pthread_mutex_t) * np);
	if (!forks)
		return (NULL);
	while(i < np)
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
	data->philos = create_philos(data);
	if (!data->philos)
		return (destroy_forks(data), free(data), NULL);
	return (data);
}

void	take_left_fork(t_philo *philo)
{
	time_t time;

	pthread_mutex_lock(&philo->mutex);
	time = get_curr_time() - philo->data->start;
	if (philo->data->die_flag)
	{
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
		printf("At :%lld\tPhilo %ld, Eating\n",time, philo->id);
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
	destroy_forks(data);
	free(data->philos);
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
