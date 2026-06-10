/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmauley <cmauley@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 22:19:03 by cmauley           #+#    #+#             */
/*   Updated: 2026/06/10 23:50:05 by cmauley          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

static int	is_dead(t_philo *philo)
{
	int	dead;

	if (safe_mutex_lock(&philo->table->data_mutex))
		return (1);
	dead = (get_time() - philo->last_meal_time
		>= philo->table->time_to_die);
	if (safe_mutex_unlock(&philo->table->data_mutex))
		return (1);
	return (dead);
}

int	monitor(t_table *table)
{
	int	i;

	while (1)
	{
		i = 0;
		while (i < table->philo_nbr)
		{
			if (is_dead(&table->philos[i]) == 1)
			{
				if (safe_mutex_lock(&table->data_mutex))
					return (1);
				table->end_simulation = 1;
				if (safe_mutex_unlock(&table->data_mutex))
					return (1);
				print_status(&table->philos[i], "died");
				return (1);
			}
			i++;
		}
	}
	return (0);
}
