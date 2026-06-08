/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmauley <cmauley@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 00:00:00 by cmauley           #+#    #+#             */
/*   Updated: 2026/06/09 00:00:00 by cmauley          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int	print_status(t_philo *philo, char *status)
{
	long	time_spend;

	if (safe_mutex_lock(&philo->table->print_mutex))
		return (1);
	time_spend = get_time() - philo->table->start_simulation;
	printf("%ld %d %s\n", time_spend, philo->id, status);
	safe_mutex_unlock(&philo->table->print_mutex);
	return (0);
}

static int	take_one_fork(t_philo *philo)
{
	int	status;

	if (safe_mutex_lock(&philo->left_fork->fork))
		return (1);
	status = print_status(philo, "has taken a fork");
	safe_mutex_unlock(&philo->left_fork->fork);
	return (status);
}

/**
 * @brief execute la routine temporaire d'un philo,
 * fonction exécutée par chaque thread
 */
void	*philo_routine(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	if (take_one_fork(philo))
		return (NULL);
	return (NULL);
}
