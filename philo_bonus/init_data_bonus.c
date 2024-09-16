/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baouragh <baouragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 13:41:14 by baouragh          #+#    #+#             */
/*   Updated: 2024/09/16 15:24:28 by baouragh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	init_philo(t_data *data , int x)
{
	data->philos.eaten_meals = 0;
	data->philos.state = 0;
	data->philos.full = 0;
	data->philos.last_meal_time = 0;
	data->philos.id = x + 1;
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
	data->philos.data = data;
	if (sem_open(&data->ps_semaphore, 1, data->num_of_philos))
		return (free(data), NULL);
	if (sem_open(&data->philos.p_semaphore, 0, 1))
		return (free(data), NULL);
	return (data);
}
