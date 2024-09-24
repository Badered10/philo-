/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   semaphores.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baouragh <baouragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 14:06:23 by baouragh          #+#    #+#             */
/*   Updated: 2024/09/23 18:33:57 by baouragh         ###   ########.fr       */
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
    sem_t               *sem_p; // forks
    sem_t               *died; // sem of check if died just in Parent
    sem_t               *full; // sem of full in child
    sem_t               *meal_sema; // sem of meal_sem in child 
    sem_t               *value_sema; // 
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
    int     *pids;
    sem_t   *died;
    bool     flag;
    sem_t   *sh_sem;
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

int get_value(sem_t *sem , sem_t *value_s)
{
    long int value;

    sem_wait(value_s);
    value = *(long int *)sem;
    sem_post(value_s);
    return (value);
}
void *cheak_death(void *infos)
{
    t_data *data;

    data = infos;
    while(1)
    {
        time_t curr = get_t() - data->start;
        sem_wait(data->meal_sema);
        if (curr - data->last_meal_time > data->ttd)
        {
            sem_wait(data->died);
            printf("%ld %ld is dead\n", curr, data->id);
            printf("--->%d :died\n", get_value(data->died, data->value_sema));
            printf("--->%d :data->meal_sema\n", get_value(data->meal_sema, data->value_sema));
            break;
        }
        sem_post(data->meal_sema);
        if(!get_value(data->full, data->value_sema))
            break;
    }
    return (NULL);
}


void *check_wait(void *data)
{
    
    t_wait *wait;
    wait = data;
    while(wait->flag) // 1
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


int main()
{
    sem_t *sem1;
    sem_t *died;
    int childs[4];
    int state;
    char *ptr;
    t_wait wait;
    // pthread_t thread;
    int x = 0;

    state = 0;
    ptr = malloc(5);
    sem_unlink(SEM_NAME_0);
    sem_unlink(SEM_NAME_1);
    sem1 = sem_open(SEM_NAME_0, O_CREAT | O_EXCL , 0644, 4);
    if (sem1 == SEM_FAILED)
    {
        perror("sem_open failed");
        exit(EXIT_FAILURE);
    }
    died = sem_open(SEM_NAME_1, O_CREAT | O_EXCL , 0644, 1);
    if (died == SEM_FAILED)
    {
        perror("sem_open failed");
        exit(EXIT_FAILURE);
    }
    wait.died = died;
    while(x < 4) // 
    {
        childs[x] = fork();
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
            sem_t       *meal_sema;
            sem_t       *value;

            sem_unlink(ft_itoa(x + 200));
            full = sem_open(ft_itoa(x + 200), O_CREAT | O_EXCL , 0644, 1);
            if(full == SEM_FAILED)
                printf("FAILED\n");

            sem_unlink(ft_itoa(x + 100));
            meal_sema = sem_open(ft_itoa(x + 100), O_CREAT | O_EXCL , 0644, 1);
            if(meal_sema == SEM_FAILED)
                printf("FAILED\n");

            sem_unlink(ft_itoa(x + 300));
            value = sem_open(ft_itoa(x + 300), O_CREAT | O_EXCL , 0644, 1);
            if(value == SEM_FAILED)
                printf("FAILED\n");
            data.sem_p = sem1;
            data.died = died;
            data.die_flag = 0;
            data.full = full;
            data.meal_sema = meal_sema;
            data.value_sema = value;
            data.num_of_meals = 10;
            data.ttd = 10;
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
                sem_wait(meal_sema);
                data.last_meal_time = get_t() - data.start; // store last meal time
                sem_post(meal_sema);
                printf("%ld %ld is eating\n", get_t() - data.start, data.id);
                data.meal_count++;
                ft_usleep(data.tte);
                sem_post(sem1); // value ++
                sem_post(sem1); // value ++
                if (data.meal_count == data.num_of_meals)
                {
                    printf("---------------> FULL\n");
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
            sem_unlink(ft_itoa(x + 100));
            printf("CHILD DONE\n");
            exit(0);
        }
        x++;
    }
    wait.pids = childs;

    sem_t   *value;

    sem_unlink(SEM_NAME_2);
    value = sem_open(SEM_NAME_2, O_CREAT | O_EXCL , 0644, 1);
    if(value == SEM_FAILED)
        printf("FAILED\n");
    wait.flag = 1;
    pthread_create(&thread, NULL, &check_wait, &wait);
    while(waitpid(-1, NULL, 0) != -1)
    ;
    wait.flag = 0;
    printf("---------------> start JOINING\n");
    pthread_join(thread, NULL);
    sem_close(sem1);
    sem_close(died);
    sem_unlink(SEM_NAME_0);
    sem_unlink(SEM_NAME_1);
    return 0;
}
