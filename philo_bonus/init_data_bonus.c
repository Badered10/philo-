/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baouragh <baouragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 13:41:14 by baouragh          #+#    #+#             */
/*   Updated: 2024/09/22 19:21:58 by baouragh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	init_philo(t_data *data , int x)
{
	data->philos.id = x + 1;
}

void	data_init(t_data **data, int argc, char **argv)
{
	(*data)->died = NULL;
	(*data)->forks = NULL;
	(*data)->sh_value = NULL;
	(*data)->list.address = &(*data);
	(*data)->list.next = NULL;
	(*data)->num_of_philos = ft_atol(argv[1]);
	(*data)->ttd = ft_atol(argv[2]);
	(*data)->tte = ft_atol(argv[3]);
	(*data)->tts = ft_atol(argv[4]);
	(*data)->num_of_meals = -10;
	if (argc == 6)
		(*data)->num_of_meals = ft_atol(argv[5]);
}

t_data	*set_data(int argc, char **argv)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (!data)
		return (NULL);
	data_init(&data, argc, argv);
	if (data->num_of_meals == -1 || data->num_of_philos == -1 || data->ttd == -1
		|| data->tte == -1 || data->tts == -1)
		return (free(data), NULL);
	memset(&data->philos, 0, sizeof(t_philo));
	data->philos.data = data;
	if(ft_sem_open(data , SEM_NAME_0, data->forks, -1))
		return (printf("sem_open faild\n"), free(data), NULL);
	if(ft_sem_open(data , SEM_NAME_1, data->died, -1))
		return (printf("sem_open faild\n"), clean_up(data, 1), NULL);
	if(ft_sem_open(data , SEM_NAME_2, data->sh_value, -1))
		return (printf("sem_open faild\n"), clean_up(data, 1), NULL);
	return (data);
}
