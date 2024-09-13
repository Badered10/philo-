/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baouragh <baouragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 18:12:59 by baouragh          #+#    #+#             */
/*   Updated: 2024/09/13 20:23:45 by baouragh         ###   ########.fr       */
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
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
		return (-1);
	return((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

long diff(long now, long start)
{
	return (now - start);
}

void	ft_usleep(time_t time)
{
	time_t	start;

	start = get_curr_time();
	while (1)
	{
		if (diff(get_curr_time(), start) >= time)
			break;
		usleep (500);
	}
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
			printf("Stop '%c' on '%s'", argv[x][y], argv[x]);
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
	while (x < data->num_of_philos) // x = 0 [0][2] , x = 1 [1][0]
	{
		philos[x].id = x + 1; //  2
		philos[x].left_fork = &data->forks[x]; // 1
		if (x + 1 == 1)
			philos[x].right_fork = &data->forks[data->num_of_philos - 1]; // 3 - 1 = 2
		else
			philos[x].right_fork = &data->forks[x - 1]; // 0
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

void	destroy_data(t_data *data)
{
	pthread_mutex_destroy(&data->write);
	pthread_mutex_destroy(&data->read);
	free(data);
}

int	init_data_mutexs(t_data *data)
{
	if (pthread_mutex_init(&data->write, NULL))
		return (printf("Faild to init a mutex\n"), 1);
	if (pthread_mutex_init(&data->read, NULL))
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
		return (destroy_data(data), NULL);
	if (argc == 6)
		data->num_of_meals = ft_atol(argv[5]);
	if (data->num_of_meals == -1 || data->num_of_philos == -1 || data->ttd == -1
		|| data->tte == -1 || data->tts == -1)
		return (destroy_data(data), NULL);
	data->forks = create_forks(data->num_of_philos);
	if (!data->forks)
		return (destroy_data(data), NULL);
	data->philos = create_philos(data);
	if (!data->philos)
		return (destroy_forks(data), destroy_data(data), NULL);
	return (data);
}

void	take_left_fork(t_philo *philo)
{
	time_t time;

	pthread_mutex_lock(philo->left_fork);
	time = get_curr_time() - philo->data->start;
	if (philo->data->die_flag)
	{
		printf("%ld %ld has taken a fork\n",time, philo->id);
	}
}

void	take_right_fork(t_philo *philo)
{
	time_t time;

	pthread_mutex_lock(philo->right_fork);
	time = get_curr_time() - philo->data->start;
	if (philo->data->die_flag)
	{
		long id =  philo->id - 2;
		if(philo->id == 1)
			id = philo->data->num_of_philos - 1;
		printf("%ld %ld has taken a fork\n",time, philo->id);
	}
}

void	eating(t_philo *philo)
{
	time_t time;

	time = get_curr_time() - philo->data->start;
	if (philo->data->die_flag)
	{
		if(time - philo->last_meal_time > philo->data->ttd)
		{
			printf("------------------------> %ld , Philo %ld DIE , time from last meal %ld, ttd %ld\n",time ,philo->id, time - philo->last_meal_time, philo->data->ttd);
			exit(1);
		}
		philo->last_meal_time = time;
		printf("%ld %ld is eating\n",time, philo->id);
		ft_usleep(philo->data->tte);
	}
}

void	take_forks(t_philo *philo)
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
}

void	put_forks(t_philo *philo)
{
	// if (philo->id % 2 == 1) // odd
	// {
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
	// }
	// else // even
	// {
	// 	pthread_mutex_unlock(philo->left_fork);
	// 	pthread_mutex_unlock(philo->right_fork);
	// }	
}

void	*philosophy(void *infos) // id = 1
{
	t_philo	*philo;

	philo = infos;
	if (philo->id % 2)
		ft_usleep(1);
	while (philo->data->die_flag)
	{
		take_forks(philo);
		eating(philo);
		put_forks(philo);
		printf("%ld %ld is sleeping\n",get_curr_time() - philo->data->start, philo->id);
		ft_usleep(philo->data->tts);
		printf("%ld %ld is thinking\n",get_curr_time() - philo->data->start, philo->id);
		if(get_curr_time() - philo->last_meal_time < philo->data->ttd)
			printf("SLEEP %ld, by %ld\n",philo->id, philo->data->tte / 2);
		if(philo->data->num_of_philos % 2)
			ft_usleep(1);
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
	data->die_flag = 1;
	data->start = get_curr_time();
	while (i < data->num_of_philos)
	{
		pthread_create(&data->philos[i].philo, NULL, &philosophy, &data->philos[i]);
		i++;
	}
	i = 0;
	while (i < data->num_of_philos)
	{
		pthread_join(data->philos[i].philo, NULL);
		i++;
	}
}

void	clean_up(t_data *data)
{
	destroy_forks(data);
	free(data->philos);
	destroy_data(data);
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
