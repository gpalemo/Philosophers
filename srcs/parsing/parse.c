/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmauley <cmauley@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 23:28:58 by cmauley           #+#    #+#             */
/*   Updated: 2026/06/08 21:12:16 by cmauley          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/philo.h"

/**
 * @brief verifie si un caractère est un chiffre
 */
static int	is_digit(char c)
{
	return (c >= '0' && c <= '9');
}

/**
 * @brief verifie si un caractère est un espace
 */
static int	is_space(char c)
{
	return ((c >= 9 && c <= 13) || c == 32);
}

/**
 * @brief verifie le début d'un argument num
 */
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
		return (NULL);
	if (!is_digit(*str))
		return (NULL);
	return (str);
}

/**
 * @brief convertit un argument valide en nombre entier
 */
static int	parse_number(const char *str, long *value)
{
	long	num;
	int		i;

	i = 0;
	num = 0;
	str = valid_input(str);
	if (!str)
		return (1);
	while (is_digit(str[i]))
	{
		if (num > (INT_MAX - (str[i] - '0')) / 10)
			return (1);
		num = num * 10 + (str[i] - '0');
		i++;
	}
	if (str[i] != '\0')
		return (1);
	*value = num;
	return (0);
}

/**
 * @brief vérifie les arguments et les enregistre dans la table
 */
int	parse_input(t_table *table, char **av)
{
	if (parse_number(av[1], &table->philo_nbr)
		|| parse_number(av[2], &table->time_to_die)
		|| parse_number(av[3], &table->time_to_eat)
		|| parse_number(av[4], &table->time_to_sleep)
		|| (av[5] && parse_number(av[5], &table->nbr_limit_meals)))
		return (print_error("Error: invalid arguments"));
	if (!av[5])
		table->nbr_limit_meals = -1;
	if (table->philo_nbr <= 0 || table->time_to_die <= 0
		|| table->time_to_eat <= 0 || table->time_to_sleep <= 0
		|| (av[5] && table->nbr_limit_meals <= 0))
		return (print_error("Error: arguments must be bigger than zero"));
	return (0);
}
