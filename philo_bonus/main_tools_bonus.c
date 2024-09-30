/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_tools_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baouragh <baouragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 14:59:25 by baouragh          #+#    #+#             */
/*   Updated: 2024/09/30 14:59:38 by baouragh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	ft_sem_forks(t_data *data, char *sem_name, t_nsem **sem, int id)
{
	char	*id_str;
	char	*name;

	(void)data;
	id_str = ft_itoa(id);
	name = ft_strjoin(sem_name, id_str);
	free(id_str);
	(*sem)->name = name;
	sem_unlink(name);
	(*sem)->sem = sem_open(name, O_CREAT | O_EXCL, 0644, id);
	if ((*sem)->sem == SEM_FAILED)
	{
		printf("FAILD TO CREAT SEM\n");
		free((*sem)->name);
		(*sem)->sem = NULL;
		return (-1);
	}
	return (0);
}

int	ft_sem_open(t_data *data, char *sem_name, t_nsem **sem, int id)
{
	char	*id_str;
	char	*name;

	(void)data;
	id_str = ft_itoa(id);
	name = ft_strjoin(sem_name, id_str);
	free(id_str);
	(*sem)->name = name;
	sem_unlink(name);
	(*sem)->sem = sem_open(name, O_CREAT | O_EXCL, 0644, 1);
	if ((*sem)->sem == SEM_FAILED)
	{
		printf("FAILD TO CREAT SEM\n");
		return (-1);
	}
	return (0);
}

int	open_sems(t_philo *philo)
{
	philo->full = malloc(sizeof(t_nsem));
	philo->meal = malloc(sizeof(t_nsem));
	philo->value = malloc(sizeof(t_nsem));
	if (ft_sem_open(philo->data, "full", &philo->full, philo->id))
		return (-1);
	if (ft_sem_open(philo->data, "meal", &philo->meal, philo->id))
		return (-1);
	if (ft_sem_open(philo->data, "value", &philo->value, philo->id))
		return (-1);
	return (0);
}
