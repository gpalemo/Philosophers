/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmauley <cmauley@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 21:53:34 by cmauley           #+#    #+#             */
/*   Updated: 2026/06/08 21:19:03 by cmauley          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/philo.h"

/**
 * @brief Affiche un message d'erreur et retourne 1.
 */
int	print_error(const char *error)
{
	printf(RED"%s\n"RST, error);
	return (1);
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
	long	i;

	i = 0;
	while (i < table->forks_initialized)
	{
		safe_mutex_destroy(&table->forks[i].fork);
		i++;
	}
	if (table->print_mutex_initialized)
		safe_mutex_destroy(&table->print_mutex);
	free(table->forks);
	free(table->philos);
}

/**
 * @brief Retourne le temps actuel en millisecondes.
 */
long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}
