/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baouragh <baouragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 05:10:28 by baouragh          #+#    #+#             */
/*   Updated: 2024/08/12 05:10:28 by baouragh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>



time_t get_curr_time(void)
{
    time_t res;
    struct timeval tv;

    res = -1;
    if (gettimeofday(&tv, NULL) == -1)
        return (-1);
    res = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
    return (res);
}

typedef struct print_args
{
    char    c;
    int     count;
}print_args;

void *print_char(void *args)
{
    int i;
    print_args *params;

    i = 0;
    params = (print_args *)args;

    while(i < params->count)
    {
        fprintf(stderr, "%c\n",params->c);
        i++;
    }
    return(NULL);

}
int ft_costume_sleep(useconds_t usec)
{
    time_t start;

    start = get_curr_time();
    usec /= 1000;
    while(get_curr_time() - start < usec)
    {
        ;
    }
    return(0);
}

void *show_time(void *args)
{
    int *count;
    time_t start;

    start = get_curr_time();
    count = args;

    while(get_curr_time() - start < *count)
    {
        printf("%ld\n",get_curr_time());
    }

    return(NULL);
}
void *timer(void *args)
{
    (void)args;

    ft_costume_sleep(10000);
    dprintf(2, "DONE %ld\n",get_curr_time());
    usleep(10000);
    dprintf(2, "DONE %ld\n",get_curr_time());

    return (NULL);
    
}

int main()
{
    
    // pthread_t id1;
    // pthread_t id2;
    pthread_t time;
    pthread_t s_time;
    // print_args arg1;
    // print_args arg2;
    
    int count;

    count = 20;
    
    bool stop;

    stop = 1;
    // arg1.c = 'x';
    // arg1.count = 0;
        
    // arg2.c = 'o';
    // arg2.count = 0;


    pthread_create(&s_time, NULL, show_time, &count);
    pthread_create(&time, NULL, timer, &stop);
    // pthread_create(&id1, NULL, print_char, &arg1);
    // pthread_create(&id2, NULL, print_char, &arg2);


    // pthread_join(id1, NULL);
    // pthread_join(id2, NULL);
    pthread_join(time, NULL);
    pthread_join(s_time, NULL);
    // stop = 0;
    // stop = 0;
    return(0);
}