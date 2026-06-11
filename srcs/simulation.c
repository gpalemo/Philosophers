/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmauley <cmauley@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 15:28:52 by cmauley           #+#    #+#             */
/*   Updated: 2026/06/11 03:45:01 by cmauley          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

/**
 * @brief initialise le temps de départ et le dernier repas des philos
 */
static void	set_start_time(t_table *table)
{
	int	i;

	table->start_simulation = get_time();
	i = 0;
	while (i < table->philo_nbr)
	{
		table->philos[i].last_meal_time = table->start_simulation;
		i++;
	}
}

/**
 * @brief crée un thread pour chaque philos
 */
static int	create_threads(t_table *table, int *created)
{
	int	i;

	i = 0;
	*created = 0;
	while (i < table->philo_nbr)
	{
		if (pthread_create(&table->philos[i].thread_id, NULL, // stocke l’id du thread dans thread_id, lance philo_routine et
				philo_routine, &table->philos[i]) != 0) // transmet l’adresse du philo concerné à la routine
			return (1);
		(*created)++; // un thread supplémentaire a correctement été créé
		i++;
	}
	return (0);
}

/**
 * @brief attend la fin de tous les threads créé
 */
static int	join_threads(t_table *table, int created)
{
	int		i;
	int		status;

	i = 0;
	status = 0;
	while (i < created)
	{
		if (pthread_join(table->philos[i].thread_id, NULL) != 0) // att la fin du thread du philo actuel.
			status = print_error("Error: pthread_join failed");
		i++;
	}
	return (status);
}

/**
 * @brief démarre puis termine proprement la simulation
 *
 * initialise le temps, crée les threads, lance le monitor si la création
 * réussi, arrete les threads si erreur puis attend leur fin
 */
int	dinner_start(t_table *table)
{
	int		created;
	int		status;

	set_start_time(table);
	status = create_threads(table, &created);
	if (status)
	{
		if (stop_simulation(table))
			status = 1;
	}
	else
		status = monitor(table);
	if (join_threads(table, created))
		status = 1;
	if (created < table->philo_nbr)
		return (print_error("Error: pthread_create failed"));
	return (status);
}
