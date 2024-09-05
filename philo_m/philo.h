/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baouragh <baouragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 18:14:24 by baouragh          #+#    #+#             */
/*   Updated: 2024/09/05 17:09:17 by baouragh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
#define PHILO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <limits.h>

#ifndef ERR_MSG
#define ERR_MSG ", Must be less than or equal to LONG_MAX"
#endif

typedef struct s_philo
{
	long		eaten_meals;
    int 		last_meal_time;
	pthread_t	*philo;
}                   t_philo;


typedef struct s_data
{
    long		num_of_philos;
    long		num_of_meals;
    long		forks;
    long		ttd;
    long		tte;
    long		tts;
    t_philo		**philos;
}                   t_data;

int	    ft_isdigit(int c);
long	ft_atol(const char *str);


#endif