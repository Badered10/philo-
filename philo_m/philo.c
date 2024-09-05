/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baouragh <baouragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 18:12:59 by baouragh          #+#    #+#             */
/*   Updated: 2024/09/05 13:44:35 by baouragh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int parse(int argc, char **argv)
{
    int x;
    int y;

    if (argc < 5 || argc > 6)
        return (printf("Must be 5 or 6 args, you entered : %d !!!\n",argc), 1);
    x = 1;
    while (argv[x])
    {
        y = 0;
        while(ft_isdigit(argv[x][y]))
            y++;
        if(argv[x][y] && !ft_isdigit(argv[x][y]))
            return (printf("Error in args.\nStop at :'%s', Please check out !\n", argv[x]) ,0);
        x++;
    }
    return (1);
}
t_data *set_data(int argc, char **argv)
{
    t_data *data;

    data = malloc(sizeof(t_data));
    if (!data)
        return (NULL);
    data->np = ft_atoi(argv[1]);
    data->ttd = ft_atoi(argv[2]);
    data->tte = ft_atoi(argv[3]);
    data->tts = ft_atoi(argv[4]);
    data->nm = -1;
    if (argc == 6)
        data->nm = ft_atoi(argv[5]);
    return (data);
}

int main(int argc, char **argv) // np ttd tte tts nm // 5 20 5 10 5
{
    t_data *data;

    if (!parse(argc, argv))
        return (1);
    data = set_data(argc, argv);
    
}