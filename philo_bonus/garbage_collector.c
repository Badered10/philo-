/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baouragh <baouragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 17:58:06 by baouragh          #+#    #+#             */
/*   Updated: 2024/09/22 21:36:54 by baouragh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void    add_back(t_garbage **list, t_garbage *node)
{
    t_garbage   *tmp;
    t_garbage   *last;

    tmp = *list;
    while (tmp)
    {
        last = tmp;
        tmp = tmp->next;
    }
    last->next = node;
}

t_garbage   *new_node(void *address)
{
    t_garbage *res;

    res = malloc(sizeof(t_garbage));
    if (!res)
        return (NULL);
    res->address = address;
    res->next = NULL;
    return (res);
}

void    free_garbage(t_garbage *list)
{
    while (list)
    {
        free(list->address);
        list = list->next;
    }
}

void    add_address(t_garbage **list, void *address)
{
    if (!list || !address)
        return;
    if (!*list)
        *list = new_node(address);
    else
        add_back(list, new_node(address));
}
