/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmauley <cmauley@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 21:30:00 by cmauley           #+#    #+#             */
/*   Updated: 2026/06/11 02:57:29 by cmauley          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

/**
 * @brief libère les deux fourchettes du philosophe
 */
int	drop_forks(t_philo *philo)
{
	int	status;

	status = 0;
	if (safe_mutex_unlock(&philo->left_fork->fork))
		status = 1;
	if (safe_mutex_unlock(&philo->right_fork->fork))
		status = 1;
	return (status);
}

/**
 * @brief fait manger le philo et compte son repas terminé
 */
int	eat(t_philo *philo)
{
	if (safe_mutex_lock(&philo->table->data_mutex))
		return (1);
	philo->last_meal_time = get_time();
	if (safe_mutex_unlock(&philo->table->data_mutex))
		return (1);
	if (print_status(philo, "is eating"))
		return (1);
	if (safe_sleep(philo->table->time_to_eat, philo->table))
		return (1);
	if (safe_mutex_lock(&philo->table->data_mutex))
		return (1);
	philo->meals_counter++;
	if (safe_mutex_unlock(&philo->table->data_mutex))
		return (1);
	return (0);
}

/**
 * @brief fait dormir et penser le philo puis espace les tables impaires
 */
int	sleep_and_think(t_philo *philo)
{
	if (print_status(philo, "is sleeping"))
		return (1);
	if (safe_sleep(philo->table->time_to_sleep, philo->table))
		return (1);
	if (print_status(philo, "is thinking"))
		return (1);
	if (philo->table->philo_nbr % 2 == 1)
		return (safe_sleep(philo->table->time_to_eat / 2, philo->table));
	return (0);
}

/**
 * @brief choisit l'ordre de prise des fourchettes selon l'id
 */
static void	choose_forks(t_philo *philo, t_fork **first, t_fork **second)
{
	if (philo->id % 2 == 0)
	{
		*first = philo->right_fork;
		*second = philo->left_fork;
	}
	else
	{
		*first = philo->left_fork;
		*second = philo->right_fork;
	}
}

/**
 * @brief verrouille et annonce la prise des fourchettes disponibles
 */
int	take_forks(t_philo *philo)
{
	t_fork	*first;
	t_fork	*second;
	int		status;

	choose_forks(philo, &first, &second);
	if (safe_mutex_lock(&first->fork))
		return (1);
	status = print_status(philo, "has taken a fork");
	if (philo->table->philo_nbr == 1)
		return (safe_mutex_unlock(&first->fork) || status);
	if (status)
		return (safe_mutex_unlock(&first->fork), 1);
	if (safe_mutex_lock(&second->fork))
		return (safe_mutex_unlock(&first->fork), 1);
	status = print_status(philo, "has taken a fork");
	if (status)
	{
		safe_mutex_unlock(&second->fork);
		safe_mutex_unlock(&first->fork);
		return (1);
	}
	return (0);
}
