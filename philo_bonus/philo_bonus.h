/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baouragh <baouragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 18:14:24 by baouragh          #+#    #+#             */
/*   Updated: 2024/09/21 22:20:56 by baouragh         ###   ########.fr       */
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


# ifndef SEM_NAME_0
#  define SEM_NAME_0 "forks"
# endif
# ifndef SEM_NAME_1
#  define SEM_NAME_1 "1"
# endif
# ifndef SEM_NAME_2
#  define SEM_NAME_2 "2"
# endif


typedef struct s_data	t_data;

typedef enum e_state
{
	NOTYET,
	DONE
}						t_state;

typedef struct s_wait
{
    int     *pids;
    sem_t   *died;
    bool     flag;
}					t_wait;

typedef struct s_philo
{
	sem_t				*full;
	sem_t				*meal;
	char				*full_name;
	char				*meal_name;
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
	sem_t				*died;
	sem_t				*forks;
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
int		ft_strlen(const char *s);
char	*ft_strjoin(char const *s1, char const *s2);

#endif