/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baouragh <baouragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 18:14:24 by baouragh          #+#    #+#             */
/*   Updated: 2024/09/15 14:17:59 by baouragh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <limits.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

# ifndef ERR_MSG
#  define ERR_MSG ", Must be less than or equal to LONG_MAX"
# endif

# ifndef ALLOC_ERR
#  define ALLOC_ERR "Allocation faild\n"
# endif

typedef struct s_data	t_data;

typedef enum e_state
{
	NOTYET,
	DONE
}						t_state;

typedef struct s_philo
{
	long				id;
	long				eaten_meals;
	long				last_meal_time;
	bool				full;
	t_state				state;
	pthread_mutex_t		*left_fork;
	pthread_mutex_t		*right_fork;
	pthread_mutex_t		meal_m;
	pthread_t			philo;
	t_data				*data;
}						t_philo;

typedef struct s_data
{
	bool				die_flag;
	long				num_of_philos;
	long				num_of_meals;
	long				ttd;
	long				tte;
	long				tts;
	pthread_mutex_t		scan;
	pthread_mutex_t		state_m;
	pthread_t			scan_t;
	time_t				start;
	pthread_mutex_t		*forks;
	t_philo				*philos;
}						t_data;

int						ft_isdigit(int c);
long					ft_atol(const char *str);
t_data					*set_data(int argc, char **argv);
int						init_data_mutexs(t_data *data);
void					destroy_data(t_data *data);
pthread_mutex_t			*create_forks(long np);
t_philo					*create_philos(t_data *data);
time_t					get_t(void);
void					clean_up(t_data *data);
int						philo_init_mutex(t_philo *philos, int x);
void					free_philos(t_data *data);
void					destroy_forks(t_data *data);
bool					get_bool(pthread_mutex_t *mutex, bool *die_flag);
void					set_bool(pthread_mutex_t *mutex, bool *die_flag,
							bool val);
int						get_state(pthread_mutex_t *mutex, t_state *state);
void					set_state(pthread_mutex_t *mutex, t_state *state,
							int new);
void					ft_usleep(time_t time, t_data *data);
void					*scan_death(void *infos);
void					get_curr_diff(t_data *data, time_t *curr, time_t *diff,
							int i);
int						all_done(t_data *data);
void					put_forks(t_philo *philo);
void					take_left_fork(t_philo *philo);
void					take_right_fork(t_philo *philo);
int						take_forks(t_philo *philo);

#endif