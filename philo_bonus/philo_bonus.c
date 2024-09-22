/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baouragh <baouragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 18:12:59 by baouragh          #+#    #+#             */
/*   Updated: 2024/09/22 19:13:17 by baouragh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	parse(int argc, char **argv)
{
	int	x;
	int	y;

	if (argc < 5 || argc > 6)
		return (printf("Must be 5 or 6 args, you entered : %d !!!\n", argc), 0);
	x = 1;
	while (argv[x])
	{
		y = 0;
		if (argv[x][y] == '+')
			y++;
		while (ft_isdigit(argv[x][y]))
			y++;
		if (argv[x][y] && !ft_isdigit(argv[x][y]))
		{
			printf("Error in args.\n");
			printf("Stop '%c' on '%s'", argv[x][y], argv[x]);
			printf(", Please check out !\n");
			return (0);
		}
		x++;
	}
	return (1);
}

int	eating(t_philo *philo)
{
	time_t	time;

	if (take_forks(philo))
		return (put_forks(philo), -1);
	time = get_t() - philo->data->start;
	sem_wait(philo->meal->sem);
	philo->last_meal_time = time;
	sem_post(philo->meal->sem);
	printf("%ld %ld is eating\n", time, philo->id);
	philo->eaten_meals++;
	if (philo->eaten_meals == philo->data->num_of_meals)
		philo->full_flag = 1;
	ft_usleep(philo->data->tte, philo->data);
	put_forks(philo);
	return (0);
}


int	ft_sem_open(t_data *data, char *sem_name, t_nsem *sem, int id)
{
	char *id_str;

	id_str = ft_itoa(id);
	add_address(&data->list, id_str);
	sem_name = ft_strjoin(sem_name, id_str);
	add_address(&data->list, sem_name);
	sem->name = sem_name;
	sem_unlink(sem_name);
	sem->sem = sem_open(sem_name, O_CREAT | O_EXCL, 0644 , 1);
	if (sem->sem == SEM_FAILED)
	{
		printf("FAILD TO CREAT SEM\n");
		return (-1);
	}
	return (0);
}

int	open_sems(t_philo *philo)
{
	if (ft_sem_open(philo->data, "full", philo->full, philo->id))
	return (-1);
	if (ft_sem_open(philo->data, "meal", philo->meal, philo->id))
	return (-1);
	if (ft_sem_open(philo->data, "value", philo->value, philo->id))
	return (-1);
	return (0);
}

void	philosophy(t_philo	*philo)
{
	if (open_sems(philo))
		clean_up(philo->data, 1);
	pthread_create(&philo->scanner, NULL, &scan_death, &philo);
	if (philo->id % 2)
		ft_usleep(1, philo->data);
	while (philo->full_flag != 1)
	{
		if (eating(philo))
			break ;
		printf("%ld %ld is sleeping\n", get_t() - philo->data->start,
			philo->id);
		ft_usleep(philo->data->tts, philo->data);
		printf("%ld %ld is thinking\n", get_t() - philo->data->start,
			philo->id);
		if (philo->data->num_of_philos % 2)
			ft_usleep(1, philo->data);
	}
	pthread_join(philo->scanner, NULL);
	clean_up(philo->data, 0);
}

int get_value(sem_t *from , sem_t *garde)
{
    long int value;

    sem_wait(garde);
    value = *(long int *)from;
    sem_post(garde);
    return (value);
}

void *check_wait(void *data)
{
    t_wait *wait;

    wait = data;
    while(get_bool(wait->sh_sem, &wait->stop))
    {
        if(get_value(wait->died, wait->sh_sem) == 0)
        {
            printf("TO KILL\n");
            int x = 0;
            while(x < 4)
            {
                printf("%d\n",wait->pids[x]);
                kill(wait->pids[x] , SIGKILL);
                x++;
            }
            break;
        }  
    }
    return (NULL);
}

void	simulation(t_data *data)
{
	int	i;
	int	pids[data->num_of_philos];
	t_wait wait;
	pthread_t thread;
	
	i = 0;
	data->start = get_t();
	while (i < data->num_of_philos)
	{
		init_philo(data , i);
		pids[i] = fork();
		if (pids[i] == 0)
			philosophy(&data->philos);
		i++;
	}
	wait.stop = 1;
    pthread_create(&thread, NULL, &check_wait, &wait);
    while(waitpid(-1, NULL, 0) != -1)
    ;
    wait.stop = 0;
    printf("---------------> start JOINING\n");
    pthread_join(thread, NULL);
}

int	main(int argc, char **argv)
{
	t_data	*data;

	if (!parse(argc, argv))
		return (1);
	data = set_data(argc, argv);
	if (!data)
		return (2);
	if (data->num_of_meals != 0)
		simulation(data);
	clean_up(data, 0);
}
