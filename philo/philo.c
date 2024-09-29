/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baouragh <baouragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 18:12:59 by baouragh          #+#    #+#             */
/*   Updated: 2024/09/29 11:56:06 by baouragh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

void	sleep_and_think(t_philo *philo)
{
	if (!get_bool(&philo->data->scan, &philo->data->die_flag))
	{
		pthread_mutex_lock(&philo->data->print);
		printf("%ld %ld is sleeping\n", get_t() - philo->start,
		philo->id);
		pthread_mutex_unlock(&philo->data->print);
	}
	ft_usleep(philo->data->tts, philo->data);
	if (!get_bool(&philo->data->scan, &philo->data->die_flag))
	{
		pthread_mutex_lock(&philo->data->print);
		printf("%ld %ld is thinking\n", get_t() - philo->start,
		philo->id);
		pthread_mutex_unlock(&philo->data->print);
	}
	if (philo->data->num_of_philos % 2)
		usleep(1000);
}

void	*philosophy(void *infos)
{
	t_philo	*philo;

	philo = infos;
	philo->start = philo->data->start;
	if (philo->id % 2)
		usleep(1000);
	while (philo->full != 1)
	{
		if (eating(philo))
			break ;
		if (philo->full)
			break;
		sleep_and_think(philo);
	}
	set_state(&philo->state_m, &philo->state, DONE);
	return (NULL);
}

void	simulation(t_data *data)
{
	int	i;

	i = 0;
	data->die_flag = 0;
	data->start = get_t();
	while (i < data->num_of_philos)
	{
		pthread_create(&data->philos[i].philo, NULL, &philosophy,
			&data->philos[i]);
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

int	main(int argc, char **argv)
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
