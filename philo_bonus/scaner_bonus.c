/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scaner_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baouragh <baouragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 13:49:06 by baouragh          #+#    #+#             */
/*   Updated: 2024/09/22 22:36:02 by baouragh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	*scan_death(void *infos)
{
	t_data *data;

    data = infos;
    while(1)
    {
        time_t curr = get_t() - data->philos.start;
        sem_wait(data->philos.meal->sem);
        if (curr - data->philos.last_meal_time > data->ttd)
        {
            sem_wait(data->died->sem);
            printf("%ld %ld is dead\n", curr, data->philos.id);
			data->philos.die_flag = 1;
            printf("--->%d :died\n", get_value(data->died->sem, data->sh_value->sem));
            printf("--->%d :data->meal_sema\n", get_value(data->philos.meal->sem, data->philos.value->sem));
            break;
        }
        sem_post(data->philos.meal->sem);
        if(!get_value(data->philos.full->sem, data->philos.value->sem))
            break;
    }
    return (NULL);
}


	// t_philo	*philo;
	// time_t	curr;
	// char	*name;
	// time_t	diff;
	// int		i;

	// i = 0;
	// philo = infos;
	// name = ft_itoa(philo->id);
	// while (1)
	// {
	// 	get_curr_diff(philo->data, &curr, &diff);
	// 	if (diff > philo->data->ttd)
	// 	{
	// 		printf("%ld %ld died\n", curr, philo->id);
	// 		sem_unlink(name);
	// 		free(name);
	// 		exit (1);
	// 	}
	// 	if (get_state(philo->p_semaphore, &philo->state) == DONE)
	// 		break ;
	// 	i++;
	// 	if (i == philo->data->num_of_philos)
	// 		i = 0;
	// }
	// return (NULL);