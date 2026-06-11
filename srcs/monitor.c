/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmauley <cmauley@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 22:19:03 by cmauley           #+#    #+#             */
/*   Updated: 2026/06/11 03:35:17 by cmauley          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

/**
 * @brief vérifie si le philo a dépassé son temps de survie
 */
static int	is_dead(t_philo *philo)
{
	int	dead;

	if (safe_mutex_lock(&philo->table->data_mutex))
		return (-1);
	dead = (get_time() - philo->last_meal_time
			>= philo->table->time_to_die);
	if (safe_mutex_unlock(&philo->table->data_mutex))
		return (-1);
	return (dead);
}

/**
 * @brief vérifie si le philo a atteint son quota de repas
 */
static int	is_full(t_philo *philo)
{
	int	full;

	if (philo->table->nbr_limit_meals == -1)
		return (0);
	if (safe_mutex_lock(&philo->table->data_mutex))
		return (-1);
	full = philo->meals_counter >= philo->table->nbr_limit_meals;
	if (safe_mutex_unlock(&philo->table->data_mutex))
		return (-1);
	return (full);
}

/**
 * @brief vérifie si tous les philos ont atteint leur quota
 */
static int	all_full(t_table *table)
{
	int	i;
	int	status;

	i = 0;
	while (i < table->philo_nbr)
	{
		status = is_full(&table->philos[i]);
		if (status == -1)
			return (-1);
		if (status == 0)
			return (0);
		i++;
	}
	return (1);
}

/**
 * @brief signale au threads que la simulation doit s'arrêter
 */
int	stop_simulation(t_table *table)
{
	if (safe_mutex_lock(&table->data_mutex))
		return (1);
	table->end_simulation = 1;
	if (safe_mutex_unlock(&table->data_mutex))
		return (1);
	return (0);
}

/**
 * @brief surveille le quota global et la mort des philos
 */
int	monitor(t_table *table)
{
	int	i;
	int	status;

	while (1)
	{
		status = all_full(table);
		if (status == -1)
			return (1);
		if (status == 1)
			return (stop_simulation(table));
		i = 0;
		while (i < table->philo_nbr)
		{
			status = is_dead(&table->philos[i]);
			if (status == -1)
				return (1);
			if (status == 1)
				return (print_death(&table->philos[i]));
			i++;
		}
		usleep(500);
	}
}
