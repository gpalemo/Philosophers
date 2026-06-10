/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmauley <cmauley@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 15:28:52 by cmauley           #+#    #+#             */
/*   Updated: 2026/06/10 23:07:27 by cmauley          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

/**
 * @brief vérifie les arguments puis lance et nettoie la simulation
 */
int	main(int ac, char **av)
{
	t_table	table;

	if (ac == 5 || ac == 6)
	{
		if (parse_input(&table, av) != 0)
			return (1);
		if (data_init(&table) != 0)
		{
			clean(&table);
			return (1);
		}
		if (dinner_start(&table) != 0)
		{
			clean(&table);
			return (1);
		}
		clean(&table);
	}
	else
	{
		print_error("Error: Wrong input");
		return (1);
	}
	return (0);
}
