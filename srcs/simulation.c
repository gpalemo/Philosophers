/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmauley <cmauley@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 15:28:52 by cmauley           #+#    #+#             */
/*   Updated: 2026/06/08 21:01:32 by cmauley          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

/**
 * @brief execute la routine temporaire d'un philo
 */
static void	*philo_routine(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	if (safe_mutex_lock(&philo->table->print_mutex))
		return (NULL);
	printf("Philosopher %d started\n", philo->id);
	safe_mutex_unlock(&philo->table->print_mutex);
	return (NULL);
}

/**
 * @brief crée les threads des philo et attend leur fin
 */
int	dinner_start(t_table *table)
{
	long	i;
	long	created;
	int		status;

	table->start_simulation = get_time();
	i = 0;
	while (i < table->philo_nbr)
	{
		table->philos[i].last_meal_time = table->start_simulation;
		i++;
	}
	i = 0;
	created = 0;
	while (i < table->philo_nbr) // crée les threads
	{
		if (pthread_create(&table->philos[i].thread_id, NULL,
				philo_routine, &table->philos[i]) != 0)
		{
			print_error("Error: pthread_create failed");
			break ;
		}
		created++;
		i++;
	}
	i = 0;
	status = 0;
	while (i < created) // attend qu'il termine
	{
		if (pthread_join(table->philos[i].thread_id, NULL) != 0)
			status = print_error("Error: pthread_join failed");
		i++;
	}
	if (created != table->philo_nbr)
		return (1);
	return (status);
}
