/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baouragh <baouragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 18:14:24 by baouragh          #+#    #+#             */
/*   Updated: 2024/09/25 09:39:53 by baouragh         ###   ########.fr       */
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
#include <sys/types.h>
#include <signal.h>

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


typedef struct s_data	t_data;

typedef enum e_state
{
	NOTYET,
	DONE
}						t_state;

typedef struct s_garbage
{
	void		*address;
	struct s_garbage	*next;
}				t_garbage;

typedef struct s_nsem
{
	sem_t	*sem;
	char	*name;
}				t_nsem;
typedef struct s_wait
{
	sem_t	*sh_sem;
    sem_t   *died;
    int     *pids;
	long	pids_num;
    bool     stop;
}					t_wait;

typedef struct s_philo
{
	t_nsem				*full;
	t_nsem				*meal;
	t_nsem              *value;
	pthread_t			scanner;
	long				id;
	long				eaten_meals;
	long				last_meal_time;
	bool				full_flag;
	bool				die_flag;
	t_state				state;
	time_t				start;
	t_data				*data;
}						t_philo;

typedef struct s_data
{
	t_garbage			*list;
	t_nsem				*died;
	t_nsem              *sh_value;
	t_nsem				*forks;
	long				num_of_philos;
	long				num_of_meals;
	long				ttd;
	long				tte;
	long				tts;
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
void	*scan_death(void *infos);
int		parse(int argc, char **argv);
int		eating(t_philo *philo);
void	philosophy(t_philo	*philo);
void	simulation(t_data *data);
void	clean_up(t_data *data, unsigned int exit_num);
time_t	get_t(void);
void	init_philo(t_data *data , int x);
t_data	*set_data(int argc, char **argv);
int		ft_isdigit(int c);
long	ft_atol(const char *str);
char	*ft_itoa(int n);
int		ft_strlen(const char *s);
char	*ft_strjoin(char const *s1, char const *s2);
int		ft_sem_open(t_data *data, char *sem_name, t_nsem **sem, int id);
void    add_address(t_garbage **list, void *address);
void    free_garbage(t_garbage *list);
long 	get_value(sem_t *from , sem_t *garde);
void	data_init(t_data **data, int argc, char **argv);
int		ft_sem_forks(t_data *data, char *sem_name, t_nsem **sem, int id);
void	set_long(sem_t *semaphore, long *varible, long val);
long	get_long(sem_t *semaphore, long *varible);

#endif