/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scaner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baouragh <baouragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 13:49:06 by baouragh          #+#    #+#             */
/*   Updated: 2024/09/15 13:49:15 by baouragh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	all_done(t_data *data)
{
	int	i;
	int	count;

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
	while (1)
	{
		get_curr_diff(data, &curr, &diff, i);
		if (diff > data->ttd)
		{
			set_bool(&data->scan, &data->die_flag, 1);
			printf("%ld %ld died\n", curr, data->philos[i].id);
			break ;
		}
		if (all_done(data))
			break ;
		i++;
		if (i == data->num_of_philos)
			i = 0;
	}
	return (NULL);
}
