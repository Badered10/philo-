/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baouragh <baouragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 13:41:14 by baouragh          #+#    #+#             */
/*   Updated: 2024/10/01 21:27:40 by baouragh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	init_philo(t_data *data, int x)
{
	data->philos.id = x + 1;
}

int	data_sem_int(t_data *data)
{
	data->died = malloc(sizeof(t_nsem));
	if (!data->died)
		return (-1);
	data->forks = malloc(sizeof(t_nsem));
	if (!data->forks)
		return (free(data->died), -1);
	data->sh_value = malloc(sizeof(t_nsem));
	if (!data->sh_value)
		return (free(data->forks), free(data->died), -1);
	data->stop = malloc(sizeof(t_nsem));
	if (!data->stop)
		return (free(data->forks), free(data->died), free(data->sh_value), -1);
	data->pids = malloc(sizeof(int) * data->num_of_philos);
	if (!data->pids)
		return (free(data->forks), free(data->died), free(data->sh_value),
			free(data->stop), -1);
	data->died->sem = NULL;
	data->forks->sem = NULL;
	data->sh_value->sem = NULL;
	data->stop->sem = NULL;
	data->died->name = NULL;
	data->forks->name = NULL;
	data->sh_value->name = NULL;
	data->stop->name = NULL;
	return (0);
}

void	data_init(t_data **data, int argc, char **argv)
{
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
	if (data_sem_int(data))
		return (free(data), NULL);
	memset(&data->philos, 0, sizeof(t_philo));
	data->philos.data = data;
	if (ft_sem_forks(data, SEM_NAME_0, &data->forks, data->num_of_philos))
		return (printf("sem_open faild\n"), clean_up(data, 1, 1), NULL);
	if (ft_sem_open(data, SEM_NAME_1, &data->died, -1))
		return (printf("sem_open faild\n"), clean_up(data, 1, 1), NULL);
	if (ft_sem_open(data, SEM_NAME_2, &data->sh_value, -1))
		return (printf("sem_open faild\n"), clean_up(data, 1, 1), NULL);
	if (ft_sem_open(data, SEM_NAME_3, &data->stop, -1))
		return (printf("sem_open faild\n"), clean_up(data, 1, 1), NULL);
	return (data);
}

void	announcement(t_philo *philo, time_t curr)
{
	sem_wait(philo->data->died->sem);
	usleep(1000);
	if (!get_bool(philo->full->sem, &philo->full_flag))
		printf("%ld %ld died\n", curr, philo->id);
}
