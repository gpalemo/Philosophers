/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmauley <cmauley@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/11 03:14:44 by cmauley           #+#    #+#             */
/*   Updated: 2026/06/11 03:22:08 by cmauley          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/philo.h"

/**
 * @brief attend une durée tout en vérifiant l'arrêt de la simulation
 */
int	safe_sleep(long duration, t_table *table)
{
	long	start;

	start = get_time();
	while (get_time() - start < duration)
	{
		if (simulation_stopped(table))
			return (1);
		usleep(500);
	}
	return (0);
}

/**
 * @brief retourne le temps actuel en ms
 */
long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}
