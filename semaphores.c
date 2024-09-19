/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   semaphores.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baouragh <baouragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 14:06:23 by baouragh          #+#    #+#             */
/*   Updated: 2024/09/19 17:22:29 by baouragh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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



typedef struct s_data
{
    char                *name;
    sem_t               *sem_p;
    sem_t               *sem;
	bool				die_flag;
    long                id;
	long				num_of_meals;
    long                last_meal_time;
	long				ttd; // 
	long				tte; // 
	long				tts; // 
	time_t				start;
}						t_data;

typedef struct s_wait
{
    int pid;
    int *pids;
}					t_wait;

#define SEM_NAME "oskd123123d2sd"

static int	lenth(int n)
{
	int	res;

	res = 1;
	if (n > 0)
		res = 0;
	while (n != 0)
	{
		n /= 10;
		res++;
	}
	return (res);
}

static char	*alloc(int res)
{
	char	*str;

	str = (char *)malloc(sizeof(char) * res + 1);
	if (!str)
		return (NULL);
	memset(str, 0 ,res + 1);
	return (str);
}

static void	fill(char *str, int num, int n)
{
	int		j;
	long	x;

	x = n;
	j = 0;
	num -= 1;
	if (x < 0)
	{
		str[0] = '-';
		x = -x;
		j++;
	}
	while (num >= j)
	{
		str[num] = x % 10 + '0';
		x /= 10;
		num--;
	}
}

char	*ft_itoa(int n)
{
	char	*res;
	int		num;

	num = lenth(n);
	res = alloc(num);
	if (!res)
		return (NULL);
	fill(res, num, n);
	return (res);
}

time_t	get_t(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
		return (-1);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	ft_usleep(time_t time)
{
	time_t	start;
	start = get_t();
	while (1)
	{
		if (get_t() - start >= time)
			break ;
		usleep(500);
		// printf("still sleeping\n");
	}
}

void *cheak_death(void *infos)
{
    t_data *data;

    data = infos;
    while(1)
    {
        time_t curr = get_t() - data->start;
        if (curr - data->last_meal_time > data->ttd)
        {
            printf("%ld %ld is dead\n", curr, data->id);
            sem_close(data->sem_p);
            exit(200);
        }
    }
    return (NULL);
}


void *check_wait(void *data)
{
    t_wait *wait;
    int state;

    wait = data;
    waitpid(wait->pid, &state, 0);
    if (state != 0)
    {
        int x = 0;
        while(x < 4)
        {
            kill(wait->pids[x] , SIGKILL);
            x++;
        }
    }
    return (NULL);
}
int main()
{
    sem_t *sem1;
    int childs[4];
    int state;
    pthread_t thread_p[4];
    int x = 0;

    state = 0;
    unlink(SEM_NAME);
    sem1 = sem_open(SEM_NAME, O_CREAT | O_EXCL, 0644, 4);
    if (sem1 == SEM_FAILED)
    {
        perror("sem_open failed");
        exit(EXIT_FAILURE);
    }
    while(x < 4)
    {
        childs[x] = fork();

        if (childs[x] < 0) 
        {
            perror("fork failed");
            sem_close(sem1);
            sem_unlink(SEM_NAME);
            exit(EXIT_FAILURE);
        }
        if (childs[x] == 0) 
        {
            pthread_t   thread;    
            t_data      data;

            data.sem_p = sem1;
            data.die_flag = 0;
            data.num_of_meals = 3;
            data.ttd = 200;
            data.tte = 1000;
            data.tts = 100;
            data.last_meal_time = 0;
            data.id = x + 1;
            data.start = get_t();
            pthread_create(&thread, NULL, &cheak_death, &data);
            pthread_detach(thread);
            if(data.id % 2)
                ft_usleep(1);
            data.start = get_t();
            while(1)
            {
                if (sem_wait(sem1) < 0)
                {
                    perror("sem_wait failed");
                    exit(EXIT_FAILURE);
                }
                printf("%ld %ld take a fork\n", get_t() - data.start, data.id);
                if (sem_wait(sem1) < 0)
                {
                    perror("sem_wait failed");
                    exit(EXIT_FAILURE);
                }
                printf("%ld %ld take a fork\n", get_t() - data.start, data.id);
                data.last_meal_time = get_t() - data.start;
                printf("%ld %ld is eating\n", get_t() - data.start, data.id);
                ft_usleep(data.tte);
                sem_post(sem1);
                sem_post(sem1);
                printf("%ld %ld is sleeping\n", get_t() - data.start, data.id);
                ft_usleep(data.tts);
                printf("%ld %ld is thinking\n", get_t() - data.start, data.id);
            }
            sem_close(sem1);
            exit(0);
        }
        x++;
    }
    x = 0;
    t_wait wait[4];

    while(x < 4)
    {
        wait[x].pid = childs[x];
        wait[x].pids = childs;
        pthread_create(&thread_p[x], NULL, &check_wait, &wait[x]);
        x++;
    }
    x = 0;
    while(x < 4)
    {
        pthread_join(thread_p[x], NULL);
        printf("JOIN\n");
        x++;
    } 
    printf("---------------> DONE");
    sem_close(sem1);
    sem_unlink(SEM_NAME);
    return 0;
}
