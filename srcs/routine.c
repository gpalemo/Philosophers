/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmauley <cmauley@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 00:00:00 by cmauley           #+#    #+#             */
/*   Updated: 2026/06/09 22:12:02 by cmauley          ###   ########.fr       */
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
 * @brief vérifie si le philo doit continuer sa routine
 */
static int	must_continue(t_philo *philo)
{
	int	continue_routine;

	if (safe_mutex_lock(&philo->table->data_mutex))
		return (0);
	if (philo->table->end_simulation == 1)
		continue_routine = 0;
	else if (philo->table->nbr_limit_meals == -1)
		continue_routine = 1;
	else if (philo->meals_counter < philo->table->nbr_limit_meals)
		continue_routine = 1;
	else
		continue_routine = 0;
	safe_mutex_unlock(&philo->table->data_mutex);
	return (continue_routine);
}

/**
 * @brief execute la routine temporaire d'un philo,
 * fonction exécutée par chaque thread
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
	while (must_continue(philo))
	{
		if (take_forks(philo))
			return (NULL);
		status = eat(philo);
		drop_forks(philo);
		if (status || sleep_and_think(philo))
			return (NULL);
	}
	return (NULL);
}
