/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baouragh <baouragh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 21:07:15 by baouragh          #+#    #+#             */
/*   Updated: 2024/09/22 19:15:33 by baouragh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*new;
	int		i;
	int		len1;
	int		len2;
	int		j;

	if (!s1 || !s2)
		return (NULL);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	i = -1;
	j = -1;
	new = (char *)malloc((len1 + len2 + 1) * sizeof(char));
	if (!new)
		return (NULL);
	while (++i < len1)
		new[i] = s1[i];
	while(++j < len2)
	{
		new[i] = s2[j];
		i++;
	}
	i--;
	new[i]= '\0';
	return (new);
}
