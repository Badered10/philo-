/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baouragh <baouragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 18:12:59 by baouragh          #+#    #+#             */
/*   Updated: 2024/09/05 17:43:07 by baouragh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void free_double(t_philo **ptr)
{
    int i;

    i = 0;
    while(ptr[i])
    {
        printf("ptr[%d]\n",i);
        if (ptr[i]->philo)
            free(ptr[i]->philo);
        free(ptr[i]);
        i++;
    }
    free(ptr);
}

int parse(int argc, char **argv)
{
    int x;
    int y;

    if (argc < 5 || argc > 6)
        return (printf("Must be 5 or 6 args, you entered : %d !!!\n",argc), 0);
    x = 1;
    while (argv[x])
    {
        y = 0;
        if(argv[x][y] == '+')
            y++;
        while(ft_isdigit(argv[x][y]))
            y++;
        if(argv[x][y] && !ft_isdigit(argv[x][y]))
            return (printf("Error in args.\nStop at :'%c' on '%s', Please check out !\n", *argv[x], argv[x]) ,0);
        x++;
    }
    return (1);
}
t_philo **create_philos(int np)
{
    t_philo **philos;
    int x;

    x = 0;
    philos = malloc(sizeof(t_philo *) * (np + 1));
    if (!philos)
        return (NULL);
    while (x < np)
    {
        philos[x] = malloc(sizeof(t_philo));
        if (!philos[x])
            return (free_double(philos), NULL);
        if(x != 3)
            philos[x]->philo = malloc(sizeof(pthread_t));
        else
            philos[x]->philo = NULL;
        if (!philos[x]->philo)
        {
            philos[x + 1] = NULL;
            return(free_double(philos), NULL);
        }
        *philos[x]->philo = 0;
        x++;
    }
    philos[x] = NULL;
    return (philos);
}
t_data *set_data(int argc, char **argv)
{
    t_data *data;

    data = malloc(sizeof(t_data));
    if (!data)
        return (NULL);
    data->num_of_philos = ft_atol(argv[1]);
    if(data->num_of_philos == -1)
        return (free(data), NULL);
    data->ttd = ft_atol(argv[2]);
    if (data->ttd == -1)
        return (free(data), NULL);
    data->tte = ft_atol(argv[3]);
    if(data->tte == -1)
        return (free(data), NULL);
    data->tts = ft_atol(argv[4]);
    if(data->tts == -1)
        return (free(data), NULL);
    data->num_of_meals = -10;
    if (argc == 6)
        data->num_of_meals= ft_atol(argv[5]);
    if(data->num_of_meals == -1)
        return (free(data), NULL);
    data->forks = data->num_of_philos;
    data->philos = create_philos(data->num_of_philos);
    if(!data->philos)
        return (free(data), NULL);
    return (data);
}

void    *philosophy(void *infos)
{
    t_data *data;
    data = infos;

    printf("A\n");
    return (NULL);
}

void    simulation(t_data *data)
{
    int i;

    i = 0;
    
    while(i < data->num_of_philos)
    {
        pthread_create(data->philos[i]->philo, NULL, &philosophy, data);
        i++;
    }
    i = 0;
    while(data->philos[i])
    {
       pthread_join(*data->philos[i]->philo, NULL);
       i++;
    }
}

int main(int argc, char **argv) // np ttd tte tts nm // 5 20 5 10 5
{
    t_data *data;

    if (!parse(argc, argv))
        return (1);
    data = set_data(argc, argv);
    if(!data)
        return (2);
    simulation(data);
}