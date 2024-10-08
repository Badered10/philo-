/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baouragh <baouragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 18:14:24 by baouragh          #+#    #+#             */
/*   Updated: 2024/10/02 18:16:00 by baouragh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <fcntl.h>
# include <limits.h>
# include <pthread.h>
# include <semaphore.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/time.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

# ifndef ERR_MSG
#  define ERR_MSG ", Must be less than or equal to LONG_MAX"
# endif

# ifndef ALLOC_ERR
#  define ALLOC_ERR "Allocation faild\n"
# endif

# ifndef SEM_NAME_0
#  define SEM_NAME_0 "forks"
# endif

# ifndef SEM_NAME_1
#  define SEM_NAME_1 "died"
# endif

# ifndef SEM_NAME_2
#  define SEM_NAME_2 "sh_value"
# endif

# ifndef SEM_NAME_3
#  define SEM_NAME_3 "stop"
# endif

typedef struct s_data	t_data;

typedef struct s_nsem
{
	sem_t				*sem;
	char				*name;
}						t_nsem;

typedef struct s_wait
{
	sem_t				*stop;
	sem_t				*died;
	int					*pids;
	long				pids_num;
	bool				stop_flag;
}						t_wait;

typedef struct s_philo
{
	t_nsem				*full;
	t_nsem				*meal;
	t_nsem				*value;
	pthread_t			scanner;
	long				id;
	long				eaten_meals;
	long				last_meal_time;
	bool				full_flag;
	bool				die_flag;
	time_t				start;
	t_data				*data;
}						t_philo;

typedef struct s_data
{
	t_nsem				*died;
	t_nsem				*sh_value;
	t_nsem				*forks;
	t_nsem				*stop;
	int					*pids;
	long				num_of_philos;
	long				num_of_meals;
	long				ttd;
	long				tte;
	long				tts;
	t_philo				philos;
}						t_data;

void					put_forks(t_philo *philo);
int						take_forks(t_philo *philo);
void					ft_usleep(time_t time, t_data *data);
void					set_bool(sem_t *semaphore, bool *die_flag, bool val);
bool					get_bool(sem_t *semaphore, bool *die_flag);
void					*scan_death(void *infos);
int						parse(int argc, char **argv);
int						eating(t_philo *philo);
void					philosophy(t_philo *philo);
void					simulation(t_data *data);
void					clean_up(t_data *data, unsigned int exit_num,
							bool mode);
time_t					get_t(void);
void					init_philo(t_data *data, int x);
t_data					*set_data(int argc, char **argv);
int						ft_isdigit(int c);
long					ft_atol(const char *str);
char					*ft_itoa(int n);
int						ft_strlen(const char *s);
char					*ft_strjoin(char const *s1, char const *s2);
int						ft_sem_open(t_data *data, char *sem_name, t_nsem **sem,
							int id);
long					get_value(sem_t *from, sem_t *garde);
void					data_init(t_data **data, int argc, char **argv);
int						ft_sem_forks(t_data *data, char *sem_name, t_nsem **sem,
							int id);
void					set_long(sem_t *semaphore, long *varible, long val);
long					get_long(sem_t *semaphore, long *varible);
long					ft_get_value(sem_t *from);
void					*check_wait(void *data);
int						open_sems(t_philo *philo);
void					announcement(t_philo *philo, time_t curr);
void					sleep_and_think(t_philo *philo);

#endif