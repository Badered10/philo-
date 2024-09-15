/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baouragh <baouragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 18:14:24 by baouragh          #+#    #+#             */
/*   Updated: 2024/09/15 08:37:12 by baouragh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <limits.h>
# include <unistd.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <stdbool.h>

# ifndef ERR_MSG
#  define ERR_MSG ", Must be less than or equal to LONG_MAX"
# endif

# ifndef ALLOC_ERR
#  define ALLOC_ERR "Allocation faild\n"
# endif

typedef struct s_data	t_data;
 
typedef enum e_state
{
	EATING,
	SLEEPING,
	THINKING,
	DIE,
	DONE
}			t_state;

typedef struct s_philo
{
	long			id;
	long			eaten_meals;
	long			last_meal_time;
	bool			full;
	t_state			state;
	pthread_mutex_t *left_fork;
	pthread_mutex_t *right_fork;
	pthread_mutex_t meal_m;
	pthread_t		philo;
	t_data			*data;
}					t_philo;

typedef struct s_data
{
	bool			die_flag;
	long			num_of_philos;
	long			num_of_meals;
	long			ttd;
	long			tte;
	long			tts;
	pthread_mutex_t scan;
	pthread_mutex_t state_m;
	pthread_t		scan_t;
	time_t			start;
	pthread_mutex_t	*forks;
	t_philo			*philos;
}					t_data;

int					ft_isdigit(int c);
long				ft_atol(const char *str);

#endif