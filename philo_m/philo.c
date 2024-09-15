/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baouragh <baouragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 18:12:59 by baouragh          #+#    #+#             */
/*   Updated: 2024/09/15 09:47:10 by baouragh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <string.h>

bool get_bool(pthread_mutex_t *mutex, bool *die_flag)
{
	bool res;

	res = 0;
	pthread_mutex_lock(mutex);
	res = *die_flag;
	pthread_mutex_unlock(mutex);
	return (res);
}

void	set_bool(pthread_mutex_t *mutex, bool *die_flag, bool val)
{
	pthread_mutex_lock(mutex);
	*die_flag = val;
	pthread_mutex_unlock(mutex);
}

int get_state(pthread_mutex_t *mutex, t_state *state)
{
	int res;

	res = -1;
	pthread_mutex_lock(mutex);
	res = *state;
	pthread_mutex_unlock(mutex);
	return (res);
}

void	set_state(pthread_mutex_t *mutex, t_state *state, int new)
{
	pthread_mutex_lock(mutex);
	*state = new;
	pthread_mutex_unlock(mutex);
}

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

void	ft_usleep(time_t time , t_data *data)
{
	time_t	start;

	start = get_curr_time();
	while (1)
	{
		if (get_curr_time() - start >= time
				|| get_bool(&data->scan, &data->die_flag))
			break;
		usleep(500);
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

void	free_philos(t_data *data)
{
	int	i;

	i = 0;
	while(i < data->num_of_philos)
	{
		pthread_mutex_destroy(&data->philos[i].meal_m);
		i++;
	}
	free(data->philos);
}

int	philo_init_mutex(t_philo *philos, int x)
{
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
		philos[x].state = 0;
		philos[x].id = x + 1; //  2
		philos[x].left_fork = &data->forks[x]; // 1
		if (x + 1 == 1)
			philos[x].right_fork = &data->forks[data->num_of_philos - 1]; // 3 - 1 = 2
		else
			philos[x].right_fork = &data->forks[x - 1]; // 0
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
	pthread_mutex_destroy(&data->scan);
	pthread_mutex_destroy(&data->state_m);
	free(data);
}

int	init_data_mutexs(t_data *data)
{
	if (pthread_mutex_init(&data->scan, NULL))
		return (printf("Faild to init a mutex\n"), 1);
	if (pthread_mutex_init(&data->state_m, NULL))
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

void	put_forks(t_philo *philo)
{
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
}

void	take_left_fork(t_philo *philo)
{
	time_t time;

	pthread_mutex_lock(philo->left_fork);
	time = get_curr_time() - philo->data->start;
	if (!get_bool(&philo->data->scan, &philo->data->die_flag))
		printf("%ld %ld has taken a fork\n",time, philo->id);
}

void	take_right_fork(t_philo *philo)
{
	time_t time;

	pthread_mutex_lock(philo->right_fork);
	time = get_curr_time() - philo->data->start;
	if (!get_bool(&philo->data->scan, &philo->data->die_flag))
		printf("%ld %ld has taken a fork\n",time, philo->id);
}

int	eating(t_philo *philo)
{
	time_t time;
	
	time = get_curr_time() - philo->data->start;
	printf("%ld %ld is eating\n",time, philo->id);
	pthread_mutex_lock(&philo->meal_m);
	philo->last_meal_time = time;
	pthread_mutex_unlock(&philo->meal_m);
	if (get_bool(&philo->data->scan, &philo->data->die_flag))
		return (put_forks(philo), -1);
	philo->eaten_meals++;
	if (philo->eaten_meals == philo->data->num_of_meals)
		philo->full = 1;
	ft_usleep(philo->data->tte, philo->data);
	return (0);
}

int	take_forks(t_philo *philo)
{
	if (get_bool(&philo->data->scan, &philo->data->die_flag))
		return (-1);
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

void	*philosophy(void *infos)
{
	t_philo	*philo;

	philo = infos;
	if (philo->id % 2)
		ft_usleep(1, philo->data);
	while (philo->full != 1)
	{
		if (take_forks(philo))
			break;
		if (eating(philo))
			break;
		put_forks(philo);
		if (philo->full == 1 ||
				get_bool(&philo->data->scan, &philo->data->die_flag))
			break;
		printf("%ld %ld is sleeping\n",get_curr_time() - philo->data->start, philo->id);
		ft_usleep(philo->data->tts, philo->data);
		if (get_bool(&philo->data->scan, &philo->data->die_flag))
			break;
		printf("%ld %ld is thinking\n",get_curr_time() - philo->data->start, philo->id);
		if(philo->data->num_of_philos % 2)
			ft_usleep(1, philo->data);
	}
	set_state(&philo->data->state_m, &philo->state, DONE);
	return (NULL);
}

int	all_done(t_data	*data)
{
	int i;
	int count;

	count = 0;
	i = 0;
	while (i < data->num_of_philos)
	{
		pthread_mutex_lock(&data->state_m);
		if (data->philos[i].state == DONE)
			count++;
		pthread_mutex_unlock(&data->state_m);
		i++;
	}
	if (count == data->num_of_philos)
		return (1);
	return (0);
}

void	stop_all(t_data *data)
{
	int i;

	i = 0;
	while (i < data->num_of_philos)
	{
		pthread_mutex_lock(&data->state_m);
		data->philos[i].state = DIE;
		pthread_mutex_unlock(&data->state_m);
		i++;
	}
}

void	*scan_death(void *infos)
{
	t_data *data;
	time_t curr;
	time_t diff;
	int i;

	i = 0;
	data = infos;
	while(true)
	{
		curr = get_curr_time() - data->start;
		pthread_mutex_lock(&data->philos[i].meal_m);
		diff = curr - data->philos[i].last_meal_time;
		pthread_mutex_unlock(&data->philos[i].meal_m);
		if (diff > data->ttd)
		{
			set_bool(&data->scan, &data->die_flag, 1);
			printf("%ld %ld died\n",curr, data->philos[i].id);
			break;
		}
		if (all_done(data))
			break;
		i++;
		if (i == data->num_of_philos)
			i = 0;
	}
	return (NULL);
}

void	simulation(t_data *data)
{
	int	i;

	i = 0;
	data->die_flag = 0;
	data->start = get_curr_time();
	while (i < data->num_of_philos)
	{
		pthread_create(&data->philos[i].philo, NULL, &philosophy, &data->philos[i]);
		i++;
	}
	pthread_create(&data->scan_t, NULL, &scan_death, data);
	i = 0;
	while (i < data->num_of_philos)
	{
		pthread_join(data->philos[i].philo, NULL);
		i++;
	}
	pthread_join(data->scan_t, NULL);
}

void	clean_up(t_data *data)
{
	destroy_forks(data);
	free_philos(data);
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
	if (data->num_of_meals != 0)
		simulation(data);
	clean_up(data);
}
