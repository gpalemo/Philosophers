/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmauley <cmauley@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 15:28:52 by cmauley           #+#    #+#             */
/*   Updated: 2026/06/08 23:58:41 by cmauley          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

/**
 * @brief définit le temps de départ de chaque philo
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
 * @brief crée un thread pour chaque philo
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
			return (print_error("Error: pthread_create failed"));
		(*created)++; // un thread supplémentaire a correctement été créé
		i++;
	}
	return (0);
}

/**
 * @brief attend la fin des threads créés
 */
static int	join_threads(t_table *table, int created)
{
	int	i;
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
 * @brief démarre les threads puis attend leur fin
 *
 * définir l’heure de départ,
 * créer les threads,
 * attendre leur fin,
 * retourner si une erreur s’est produite
 */
int	dinner_start(t_table *table)
{
	int		created;
	int		status;

	set_start_time(table);
	status = create_threads(table, &created);
	if (join_threads(table, created))
		status = 1;
	return (status);
}
