/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmauley <cmauley@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 00:00:00 by cmauley           #+#    #+#             */
/*   Updated: 2026/06/09 00:11:03 by cmauley          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

/**
 * @brief affiche le statut d'un philo avec le temps écoulé
 */
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

/**
 * @brief fait prendre puis relâcher une fourchette au philo
 */
static int	take_forks(t_philo *philo)
{
	t_fork	*first;
	t_fork	*second;
	int		status;

	if (philo->id % 2 == 0)
	{
		first = philo->right_fork;
		second = philo->left_fork;
	}
	else
	{
		first = philo->left_fork;
		second = philo->right_fork;
	}
	(void)second;
	if (safe_mutex_lock(&first->fork))
		return (1);
	status = print_status(philo, "has taken a fork");
	safe_mutex_unlock(&first->fork);
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
	if (take_forks(philo))
		return (NULL);
	return (NULL);
}
