/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baouragh <baouragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 18:12:59 by baouragh          #+#    #+#             */
/*   Updated: 2024/07/12 11:21:55 by baouragh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"philo.h"

int parse(char **argv)
{
    int x;
    int y;

    x = 1;
    while(argv[x])
    {
        y = 0;
        while(ft_isdigit(argv[x][y]))
            y++;
        if(!ft_isdigit(argv[x][y]) && argv[x][y])
            return (0);
        x++;
    }
    return(1);
}
// t_data creat_data(int argc, char **argv)
// {
    
// }

int main(int argc, char **argv) // np ttd tte tts nm // 5 20 5 10 5
{
    if(argc < 5 || argc > 6)
        return (printf("Must be 5 or 6 args, you entered : %d !!!\n",argc), 1);
    if(!parse(argv))
        return(printf("Error in args please check out !\n"), 1);
    
}