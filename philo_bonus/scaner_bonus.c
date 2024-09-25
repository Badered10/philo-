/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scaner_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baouragh <baouragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 13:49:06 by baouragh          #+#    #+#             */
/*   Updated: 2024/09/25 11:18:06 by baouragh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	get_curr_diff(t_philo *philo , time_t *curr , time_t *diff)
{
	time_t last;

	last = get_long(philo->meal->sem, &philo->last_meal_time);
	*curr = get_t() - philo->start;
	*diff = *curr - last;
}

void	*scan_death(void *infos)
{
	t_philo *philo;
	time_t	diff;
	time_t	curr;

    philo = infos;
    while(!get_bool(philo->full->sem, &philo->full_flag))
    { 
		get_curr_diff(philo , &curr , &diff);
        if (diff > philo->data->ttd)
        {
            printf("%ld %ld is dead\n", curr, philo->id);
			sem_wait(philo->data->died->sem);
			set_bool(philo->value->sem, &philo->die_flag, 1);
            break;
        }
        if(!get_value(philo->full->sem, philo->value->sem))
            break;
		usleep(500);
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