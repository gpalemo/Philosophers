/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmauley <cmauley@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 15:28:52 by cmauley           #+#    #+#             */
/*   Updated: 2026/06/01 23:27:59 by cmauley          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int ac, char **av)
{
	t_table	table;

	if (ac == 5 || ac == 6)
	{
		// correct input
		//1) errors checking, filling table table
		parse_input(&table, av);

		//2) creating the actual thing
		data_init(&table);

		//3)
		dinner_start(&table);

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
