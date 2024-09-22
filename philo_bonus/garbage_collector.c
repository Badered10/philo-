/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baouragh <baouragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 17:58:06 by baouragh          #+#    #+#             */
/*   Updated: 2024/09/22 19:26:59 by baouragh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

t_garbage   *new_node(void   *address)
{
    t_garbage *res;

    res = malloc(sizeof(t_garbage));
    if (!res)
        return (NULL);
    res->address = address;
    res->next = NULL;
}

void    free_garbage(t_garbage *list)
{
    while (list)
    {
        free(list->address);
        list = list->next;
    }
}

void    add_address(t_garbage **list, t_garbage *node)
{
    if (!list || !address)
        return;
    if (!*list)
    {
        *list = address;
        (*list)->next = NULL;
    }
    (*list)->next = address;
    (*list)->next->next = NULL;
}
