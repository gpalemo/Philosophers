/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmauley <cmauley@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 15:28:52 by cmauley           #+#    #+#             */
/*   Updated: 2026/06/08 21:01:13 by cmauley          ###   ########.fr       */
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
		// correct input
		//1) errors checking, filling table table
		if (parse_input(&table, av) != 0)
			return (1);

		//2) creating the actual thing
		if (data_init(&table) != 0)
		{
			clean(&table);
			return (1);
		}

		//3)
		if (dinner_start(&table) != 0)
		{
			clean(&table);
			return (1);
		}

		//4) No leaks -> philos full | 1 philo died 💀
		clean(&table);
	}
	else
	{
		print_error("Wrong input\n");
		return (1);
	}
	return (0);
}
