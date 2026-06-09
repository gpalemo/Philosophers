/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmauley <cmauley@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 21:30:00 by cmauley           #+#    #+#             */
/*   Updated: 2026/06/09 22:01:24 by cmauley          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	drop_forks(t_philo *philo)
{
	safe_mutex_unlock(&philo->left_fork->fork);
	safe_mutex_unlock(&philo->right_fork->fork);
}

int	eat(t_philo *philo)
{
	if (safe_mutex_lock(&philo->table->data_mutex))
		return (1);
	philo->last_meal_time = get_time();
	if (safe_mutex_unlock(&philo->table->data_mutex))
		return (1);
	if (print_status(philo, "is eating"))
		return (1);
	usleep(philo->table->time_to_eat * 1000);
	if (safe_mutex_lock(&philo->table->data_mutex))
		return (1);
	philo->meals_counter++;
	if (safe_mutex_unlock(&philo->table->data_mutex))
		return (1);
	return (0);
}

int	sleep_and_think(t_philo *philo)
{
	if (print_status(philo, "is sleeping"))
		return (1);
	usleep(philo->table->time_to_sleep * 1000);
	if (print_status(philo, "is thinking"))
		return (1);
	return (0);
}

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
 * @brief fait prendre les fourchettes au philo
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
	{
		safe_mutex_unlock(&first->fork);
		return (status);
	}
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
