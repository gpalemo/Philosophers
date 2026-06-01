/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmauley <cmauley@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 23:28:58 by cmauley           #+#    #+#             */
/*   Updated: 2026/06/02 00:27:11 by cmauley          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	is_digit(char c)
{
	return (c >= '0' && c <= '9');
}

static int	is_space(char c)
{
	return ((c >= 9 && c <= 13) || c == 32);
}

static const char	*valid_input(const char *str)
{
	int	i;

	i = 0;
	while (is_space(str[i]))
		i++;
	str = &str[i];
	if (*str == '+')
		str++;
	else if (*str == '-')
	{
		print_error("Error: negative numbers not allowed");
		return (NULL);
	}
	if (!is_digit(*str))
	{
		print_error("Error: not a valid digit");
		return (NULL);
	}
	return (str);
}

static long	ft_atol(const char *str)
{
	long	num;
	int		i;

	i = 0;
	num = 0;
	str = valid_input(str);
	if (!str)
		return (-1);
	while (is_digit(str[i]))
	{
		num = num * 10 + (str[i] - '0');
		i++;
	}
	if (num > INT_MAX)
		print_error("Error: value exceeds INT_MAX");
	return (num);
}

void	parse_input(t_table *table, char **av)
{
	table->philo_nbr = ft_atol(av[1]);
	table->time_to_die = ft_atol(av[2]);
	table->time_to_eat = ft_atol(av[3]);
	table->time_to_sleep = ft_atol(av[4]);
	if (av[5])
		table->nbr_limit_meals = ft_atol(av[5]);
	else
		table->nbr_limit_meals = -1;
}

