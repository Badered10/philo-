/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scaner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baouragh <baouragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 13:49:06 by baouragh          #+#    #+#             */
/*   Updated: 2024/10/01 20:55:35 by baouragh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	all_done(t_data *data) // to fix
{
	int	i;
	int	count;

	count = 0;
	i = 0;
	while (i < data->num_of_philos)
	{
		pthread_mutex_lock(&data->philos[i].state_m);
		if (data->philos[i].state == DONE)
			count++;
		pthread_mutex_unlock(&data->philos[i].state_m);
		i++;
	}
	if (count == data->num_of_philos)
		return (1);
	return (0);
}

void	get_curr_diff(t_data *data, time_t *curr, time_t *diff, int i)
{
	*curr = get_t() - data->start;
	pthread_mutex_lock(&data->philos[i].meal_m);
	*diff = *curr - data->philos[i].last_meal_time;
	pthread_mutex_unlock(&data->philos[i].meal_m);
}

void	*scan_death(void *infos)
{
	t_data	*data;
	time_t	curr;
	time_t	diff;
	int		i;

	i = 0;
	data = infos;
	while (!all_done(data))
	{
		get_curr_diff(data, &curr, &diff, i);
		if (diff > data->ttd)
		{
			set_bool(&data->scan, &data->die_flag, 1);
			usleep(1000);
			printf("%ld %ld died\n", curr, data->philos[i].id);
			break ;
		}
		if (all_done(data))
			break ;
		i++;
		if (i == data->num_of_philos)
			i = 0;
		usleep(500);
	}
	return (NULL);
}

// int	init_print_mutex(t_philo *philos, int x)
// {
// 	if (pthread_mutex_init(&philos[x].print, NULL))
// 	{
// 		while (x > 0)
// 		{
// 			pthread_mutex_destroy(&philos[x].print);
// 			x--;
// 		}
// 		free(philos);
// 		return (printf("Failed to create a mutex philos !\n"), -1);
// 	}
// 	return (0);
// }
