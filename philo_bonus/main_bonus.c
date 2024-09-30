/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baouragh <baouragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 18:12:59 by baouragh          #+#    #+#             */
/*   Updated: 2024/09/30 15:00:14 by baouragh         ###   ########.fr       */
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
	time_t	time;

	if (take_forks(philo))
		return (put_forks(philo), -1);
	sem_wait(philo->meal->sem);
	time = get_t() - philo->start;
	philo->last_meal_time = time;
	sem_post(philo->meal->sem);
	sem_wait(philo->value->sem);
	if (get_value(philo->data->died->sem, philo->data->sh_value->sem))
		printf("%ld %ld is eating\n", time, philo->id);
	sem_post(philo->value->sem);
	philo->eaten_meals++;
	if (philo->eaten_meals == philo->data->num_of_meals)
		set_bool(philo->full->sem, &philo->full_flag, 1);
	ft_usleep(philo->data->tte, philo->data);
	put_forks(philo);
	if (get_bool(philo->full->sem, &philo->full_flag))
		return (1);
	return (0);
}

void	philosophy(t_philo *philo)
{
	if (open_sems(philo))
		clean_up(philo->data, 1, 0);
	pthread_create(&philo->scanner, NULL, &scan_death, philo);
	if (philo->id % 2)
		usleep(1000);
	while (get_bool(philo->full->sem, &philo->full_flag) != 1
		|| !get_value(philo->data->died->sem, philo->data->sh_value->sem))
	{
		if (eating(philo))
			break ;
		sem_wait(philo->value->sem);
		if (get_value(philo->data->died->sem, philo->data->sh_value->sem))
			printf("%ld %ld is sleeping\n", get_t() - philo->start, philo->id);
		sem_post(philo->value->sem);
		ft_usleep(philo->data->tts, philo->data);
		sem_wait(philo->value->sem);
		if (get_value(philo->data->died->sem, philo->data->sh_value->sem))
			printf("%ld %ld is thinking\n", get_t() - philo->start, philo->id);
		sem_post(philo->value->sem);
		if (philo->data->num_of_philos % 2)
			usleep(1000);
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
	data->philos.start = get_t();
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
