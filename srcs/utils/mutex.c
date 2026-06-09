/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmauley <cmauley@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 18:43:48 by cmauley           #+#    #+#             */
/*   Updated: 2026/06/08 21:19:53 by cmauley          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

/**
 * @brief initialise un mutex et vérifie le résultat
 */
int	safe_mutex_init(t_mtx *mutex)
{
	if (pthread_mutex_init(mutex, NULL) != 0)
	{
		print_error("Error: pthread_mutex_init failed");
		return (1);
	}
	return (0);
}

/**
 * @brief détruit un mutex et vérifie le résultat
 */
int	safe_mutex_destroy(t_mtx *mutex)
{
	if (pthread_mutex_destroy(mutex) != 0)
	{
		print_error("Error: pthread_mutex_destroy failed");
		return (1);
	}
	return (0);
}

/**
 * @brief verrouille un mutex et vérifie le résultat
 */
int	safe_mutex_lock(t_mtx *mutex)
{
	if (pthread_mutex_lock(mutex) != 0)
	{
		print_error("Error: pthread_mutex_lock failed");
		return (1);
	}
	return (0);
}

/**
 * @brief deverrouille un mutex et verifie le résultat
 */
int	safe_mutex_unlock(t_mtx *mutex)
{
	if (pthread_mutex_unlock(mutex) != 0)
	{
		print_error("Error: pthread_mutex_unlock failed");
		return (1);
	}
	return (0);
}
