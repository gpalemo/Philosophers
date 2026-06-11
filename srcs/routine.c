/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmauley <cmauley@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 00:00:00 by cmauley           #+#    #+#             */
/*   Updated: 2026/06/11 03:14:22 by cmauley          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

/**
 * @brief affiche un statut si la simulation est encore active
 */
int	print_status(t_philo *philo, char *status)
{
	long	time_spend;
	int		stopped;

	if (safe_mutex_lock(&philo->table->print_mutex))
		return (1);
	if (safe_mutex_lock(&philo->table->data_mutex))
		return (safe_mutex_unlock(&philo->table->print_mutex), 1);
	stopped = philo->table->end_simulation;
	if (safe_mutex_unlock(&philo->table->data_mutex))
		return (safe_mutex_unlock(&philo->table->print_mutex), 1);
	time_spend = get_time() - philo->table->start_simulation;
	if (!stopped)
		printf("%ld %d %s\n", time_spend, philo->id, status);
	if (safe_mutex_unlock(&philo->table->print_mutex))
		return (1);
	return (stopped);
}

/**
 * @brief exécute la routine du philo jusqu'à la fin de la simulation
 */
void	*philo_routine(void *data)
{
	t_philo	*philo;
	int		status;

	philo = (t_philo *)data;
	if (philo->table->philo_nbr == 1)
	{
		take_forks(philo);
		return (NULL);
	}
	if (philo->id % 2 == 0)
		if (safe_sleep(philo->table->time_to_eat / 2, philo->table))
			return (NULL);
	while (!simulation_stopped(philo->table))
	{
		if (take_forks(philo))
			return (NULL);
		status = eat(philo);
		if (drop_forks(philo))
			return (NULL);
		if (status || sleep_and_think(philo))
			return (NULL);
	}
	return (NULL);
}
