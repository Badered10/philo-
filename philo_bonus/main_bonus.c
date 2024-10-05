/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baouragh <baouragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 18:12:59 by baouragh          #+#    #+#             */
/*   Updated: 2024/10/05 14:28:40 by baouragh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

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
	if (take_forks(philo))
		return (put_forks(philo), -1);
	if (get_value(philo->data->died->sem, philo->data->sh_value->sem))
	{
		sem_wait(philo->value->sem);
		printf("%ld %ld is eating\n", get_t() - philo->start, philo->id);
		sem_post(philo->value->sem);
	}
	sem_wait(philo->meal->sem);
	philo->last_meal_time = get_t() - philo->start;
	sem_post(philo->meal->sem);
	philo->eaten_meals++;
	if (philo->eaten_meals == philo->data->num_of_meals)
	{
		set_bool(philo->full->sem, &philo->full_flag, 1);
		ft_usleep(philo->data->tte, philo->data);
		put_forks(philo);
		return (1);
	}
	ft_usleep(philo->data->tte, philo->data);
	put_forks(philo);
	return (0);
}

void	philosophy(t_philo *philo)
{
	if (open_sems(philo))
		clean_up(philo->data, 1, 0);
	while (get_t() < philo->start)
		usleep(500);
	philo->start = get_t();
	pthread_create(&philo->scanner, NULL, &scan_death, philo);
	if (philo->id % 2)
		usleep(2000);
	while (1)
	{
		if (eating(philo))
			break ;
		sleep_and_think(philo);
	}
	pthread_join(philo->scanner, NULL);
	clean_up(philo->data, 0, 0);
}

void	simulation(t_data *data)
{
	int			i;
	pthread_t	thread;
	t_wait		wait;

	i = 0;
	data->philos.start = get_t() + data->num_of_philos * 25;
	while (i < data->num_of_philos)
	{
		init_philo(data, i);
		data->pids[i] = fork();
		if (data->pids[i] == 0)
			philosophy(&data->philos);
		i++;
	}
	wait.died = data->died->sem;
	wait.stop = data->stop->sem;
	wait.stop_flag = 1;
	wait.pids = data->pids;
	wait.pids_num = data->num_of_philos;
	pthread_create(&thread, NULL, &check_wait, &wait);
	while (waitpid(-1, NULL, 0) != -1)
		;
	set_bool(data->stop->sem, &wait.stop_flag, 0);
	pthread_join(thread, NULL);
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
	clean_up(data, 0, 1);
}
