/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baouragh <baouragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 18:14:24 by baouragh          #+#    #+#             */
/*   Updated: 2024/09/16 16:47:04 by baouragh         ###   ########.fr       */
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
#include <semaphore.h>
#include <sys/wait.h>
#include <fcntl.h> 
#include <sys/stat.h> 

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
	sem_t				*p_semaphore;
	pthread_t			scanner;
	long				id;
	long				eaten_meals;
	long				last_meal_time;
	bool				full;
	t_state				state;
	t_data				*data;
}						t_philo;

typedef struct s_data
{
	sem_t				*ps_semaphore;
	bool				die_flag;
	long				num_of_philos;
	long				num_of_meals;
	long				ttd;
	long				tte;
	long				tts;
	time_t				start;
	t_philo				philos;
}						t_data;

void	put_forks(t_philo *philo);
void	take_fork(t_philo *philo);
int		take_forks(t_philo *philo);
void	ft_usleep(time_t time, t_data *data);
void	set_state(sem_t *semaphore, t_state *state, int new);
int		get_state(sem_t *semaphore, t_state *state);
void	set_bool(sem_t *semaphore, bool *die_flag, bool val);
bool	get_bool(sem_t *semaphore, bool *die_flag);
void	get_curr_diff(t_data *data, time_t *curr, time_t *diff);
void	*scan_death(void *infos);
int		parse(int argc, char **argv);
int		eating(t_philo *philo);
void	philosophy(t_philo	*philo);
void	simulation(t_data *data);
int		main(int argc, char **argv);
void	clean_up(t_data *data);
time_t	get_t(void);
void	init_philo(t_data *data , int x);
t_data	*set_data(int argc, char **argv);
int		ft_isdigit(int c);
long	ft_atol(const char *str);
char	*ft_itoa(int n);

#endif