/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmauley <cmauley@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 21:53:34 by cmauley           #+#    #+#             */
/*   Updated: 2026/06/11 03:21:50 by cmauley          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/philo.h"

/**
 * @brief affiche un message d'erreur puis retourne 1
 */
int	print_error(const char *error)
{
	printf(RED"%s\n"RST, error);
	return (1);
}

/**
 * @brief arrête la simulation et affiche la mort du philosophe
 */
int	print_death(t_philo *philo)
{
	long	time_spend;

	if (safe_mutex_lock(&philo->table->print_mutex))
		return (1);
	if (safe_mutex_lock(&philo->table->data_mutex))
		return (safe_mutex_unlock(&philo->table->print_mutex), 1);
	philo->table->end_simulation = 1;
	if (safe_mutex_unlock(&philo->table->data_mutex))
		return (safe_mutex_unlock(&philo->table->print_mutex), 1);
	time_spend = get_time() - philo->table->start_simulation;
	printf("%ld %d died\n", time_spend, philo->id);
	if (safe_mutex_unlock(&philo->table->print_mutex))
		return (1);
	return (0);
}

/**
 * @brief alloue de la mémoire et affiche une erreur en cas d'échec
 */
void	*safe_malloc(size_t bytes)
{
	void	*res;

	res = malloc(bytes);
	if (res == NULL)
		print_error("Error: malloc failed");
	return (res);
}

/**
 * @brief détruit les mutex et libère les données allouées
 */
void	clean(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->forks_initialized)
	{
		safe_mutex_destroy(&table->forks[i].fork);
		i++;
	}
	if (table->data_mutex_initialized)
		safe_mutex_destroy(&table->data_mutex);
	if (table->print_mutex_initialized)
		safe_mutex_destroy(&table->print_mutex);
	free(table->forks);
	free(table->philos);
}

/**
 * @brief lit de façon protégée si la simulation doit s'arrêter
 */
int	simulation_stopped(t_table *table)
{
	int	stopped;

	if (safe_mutex_lock(&table->data_mutex))
		return (1);
	stopped = table->end_simulation;
	if (safe_mutex_unlock(&table->data_mutex))
		return (1);
	return (stopped);
}
