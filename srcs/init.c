/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmauley <cmauley@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 15:28:52 by cmauley           #+#    #+#             */
/*   Updated: 2026/06/11 03:36:33 by cmauley          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

/**
 * @brief met les données d'initialisation de la table à zéro
 */
static void	set_table_to_zero(t_table *table)
{
	table->end_simulation = 0; // simulation pas finie (si vaut 1, doit sarreter)
	table->start_simulation = 0; // sera def avant de lancer les threads
	table->philos = NULL;
	table->forks = NULL;
	table->forks_initialized = 0;
	table->print_mutex_initialized = 0;
	table->data_mutex_initialized = 0;
}

/**
 * @brief initialise chaque fourchette
 */
static int	init_forks(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->philo_nbr)
	{
		table->forks[i].fork_id = i;
		if (safe_mutex_init(&table->forks[i].fork)) // empêche deux philo de prendre la même fourchette en mm temps
			return (1);
		table->forks_initialized++;
		i++;
	}
	return (0);
}

/**
 * @brief initialise les données de chaque philo
 */
static void	init_philos(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->philo_nbr)
	{
		table->philos[i].id = i + 1;
		table->philos[i].meals_counter = 0;
		table->philos[i].full = 0;
		table->philos[i].last_meal_time = 0;
		table->philos[i].left_fork = &table->forks[i];
		table->philos[i].right_fork
			= &table->forks[(i + 1) % table->philo_nbr];
		table->philos[i].table = table;
		i++;
	}
}

/**
 * @brief alloue et initialise la table, les fourchettes et les philo
 */
int	data_init(t_table *table)
{
	set_table_to_zero(table);
	table->philos = safe_malloc(sizeof(t_philo) * table->philo_nbr);
	table->forks = safe_malloc(sizeof(t_fork) * table->philo_nbr);
	if (!table->philos || !table->forks)
		return (1);
	if (safe_mutex_init(&table->print_mutex))
		return (1);
	table->print_mutex_initialized = 1;
	if (safe_mutex_init(&table->data_mutex))
		return (1);
	table->data_mutex_initialized = 1;
	if (init_forks(table))
		return (1);
	init_philos(table);
	return (0);
}
