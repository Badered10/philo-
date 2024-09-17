/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   semaphores.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baouragh <baouragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 14:06:23 by baouragh          #+#    #+#             */
/*   Updated: 2024/09/17 14:16:53 by baouragh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus/philo_bonus.h"

#define name1 "/var/tmp/sem1"
#define name2 "/var/tmp/sem2"

int main()
{
    sem_t *sem1;
    int x = 0;
    // sem_t *sem2;

    sem1 = sem_open(name1, O_CREAT | O_EXCL | S_IRUSR | S_IWUSR , 0);
    printf("before loop\n");
    while(x != 2)
    {
        printf("for x %d \n",x);
        int id = fork();
        if(!id)
        {
            int x = 0;
            while(x != 2)
            {
                printf("ENTERD\n");
                sem_wait(sem1);
                printf("1\n");
                sem_post(sem1);
                usleep(2000);
                printf("sleeped\n");
                x++;
            }
            sem_close(sem1);
            exit(0);    
        }
        x++;
    }
    while(waitpid(-1, NULL, 0) != -1)
        ;
    sem_close(sem1);
    sem_unlink(name1);
}