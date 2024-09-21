/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   semaphores.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baouragh <baouragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 14:06:23 by baouragh          #+#    #+#             */
/*   Updated: 2024/09/21 10:20:21 by baouragh         ###   ########.fr       */
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
    char                *dead_name;
    sem_t               *sem_p; // 
    sem_t               *died; //
    sem_t               *full; //
    sem_t               *sem; // mutex like
	bool				die_flag;
    long                id;
    long                meal_count;
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

#define SEM_NAME_0 "1"
#define SEM_NAME_1 "2"
#define SEM_NAME_2 "3"

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

int get_value(sem_t *sem)
{
    // printf("%ld\n", *(long int *)sem);
    return(*(long int *)sem);
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
            sem_wait(data->died);
            // sem_close(data->sem_p);
            // exit(200);
            break;
        }
        if(!get_value(data->full))
            break;
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
    sem_t *died;
    int childs[4];
    int state;
    // pthread_t thread_p[4];
    int x = 0;

    state = 0;
    sem_unlink(SEM_NAME_0);
    sem_unlink(SEM_NAME_1);
    sem1 = sem_open(SEM_NAME_0, O_CREAT , 0644, 4);
    if (sem1 == SEM_FAILED)
    {
        perror("sem_open failed");
        exit(EXIT_FAILURE);
    }
    died = sem_open(SEM_NAME_1, O_CREAT , 0644, 1);
    if (died == SEM_FAILED)
    {
        perror("sem_open failed");
        exit(EXIT_FAILURE);
    }
    while(x < 4) // 
    {
        childs[x] = fork();
        printf("process : %d, int : %d\n",getpid(), childs[x]);
        if (childs[x] < 0) 
        {
            perror("fork failed");
            sem_close(sem1);
            sem_unlink(SEM_NAME_0);
            exit(EXIT_FAILURE);
        }
        if (childs[x] == 0) 
        {
            pthread_t   thread; // thread check death
            t_data      data;
            sem_t       *full;

            sem_unlink(ft_itoa(x + 200));
            full = sem_open(ft_itoa(x + 200), O_CREAT , 0644, 1);
            data.sem_p = sem1;
            data.died = died;
            data.die_flag = 0;
            data.full = full;
            data.num_of_meals = 10;
            data.ttd = 180;
            data.tte = 60;
            data.tts = 60;
            data.last_meal_time = 0;
            data.id = x + 1;
            data.meal_count = 0;
            data.start = get_t();
            pthread_create(&thread, NULL, &cheak_death, &data);
            pthread_detach(thread);
            if(data.id % 2)
                ft_usleep(1);
            data.start = get_t();
            while(1)
            {
                if (sem_wait(sem1) < 0) // take fork value --
                {
                    perror("sem_wait failed");
                    exit(EXIT_FAILURE);
                }
                printf("%ld %ld take a fork\n", get_t() - data.start, data.id);
                if (sem_wait(sem1) < 0) // take fork value --
                {
                    perror("sem_wait failed");
                    exit(EXIT_FAILURE);
                }
                printf("%ld %ld take a fork\n", get_t() - data.start, data.id);
                data.last_meal_time = get_t() - data.start; // store last meal time
                printf("%ld %ld is eating\n", get_t() - data.start, data.id);
                data.meal_count++;
                ft_usleep(data.tte);
                sem_post(sem1); // value ++
                sem_post(sem1); // value ++
                if (data.meal_count == data.num_of_meals)
                {
                    sem_wait(full);
                    break;
                }
                printf("%ld %ld is sleeping\n", get_t() - data.start, data.id);
                ft_usleep(data.tts);
                printf("%ld %ld is thinking\n", get_t() - data.start, data.id);
               	if (4 % 2)
			        ft_usleep(1);
            }
            sem_close(sem1);
            sem_close(full);
            sem_close(died);
            sem_unlink(ft_itoa(x + 200));
            exit(0);
        }
        x++;
    }
    
    while(waitpid(-1, NULL, 0) != -1)
    {
        if(get_value(died) == 0)
        {
            printf("TO KILL\n");
            int x = 0;
            while(x < 4)
            {
                kill(childs[x] , SIGKILL);
                x++;
            }
            break;
        }
    }
    // x = 0;
    // t_wait wait[4]; // id = pid, int *pids;

    // while(x < 4)
    // {
    //     wait[x].pid = childs[x]; // pid to wait
    //     wait[x].pids = childs; // all pids in case of kill
    //     pthread_create(&thread_p[x], NULL, &check_wait, &wait[x]);
    //     x++;
    // }
    // x = 0;
    // while(x < 4)
    // {
    //     pthread_join(thread_p[x], NULL);
    //     printf("JOIN\n");
    //     x++;
    // } 
    printf("---------------> DONE");
    sem_close(sem1);
    sem_close(died);
    sem_unlink(SEM_NAME_0);
    sem_unlink(SEM_NAME_1);
    return 0;
}
