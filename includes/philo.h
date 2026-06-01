/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmauley <cmauley@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 21:16:36 by cmauley           #+#    #+#             */
/*   Updated: 2026/06/02 00:48:41 by cmauley          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <time.h> // gettimeofday
# include <pthread.h> // mutex : init, destroy, lock, unlock | threads : create, join, detach
# include <string.h>
# include <stdbool.h>
# include <stdio.h> // printf
# include <stdlib.h> // malloc, free
# include <unistd.h> // write, usleep
# include <limits.h> // INT MIN, INT MAX

/*
** ANSI Escape Sequences pour les textes en couleur
** Utilisation:
**      printf(R "c'est un texte rouge." RST);
**      printf(B "c'est un texte bleu." RST);
** Ne pas oublier dutiliser RST pour reset la couleur.
*/

# define RST	"\033[0m"	/* Réinitialiser à la couleur par défaut */
# define RED	"\033[1;31m"	/* Rouge gras */
# define G	"\033[1;32m"	/* Vert gras */
# define Y	"\033[1;33m"	/* Jaune gras */
# define B	"\033[1;34m"	/* Bleu gras */
# define M	"\033[1;35m"	/* Magenta gras */
# define C	"\033[1;36m"	/* Cyan gras */
# define W	"\033[1;37m"	/* Blanc gras */


typedef pthread_mutex_t	t_mtx;

// c bon ca existe trdc
typedef struct s_table	t_table;

// FORK
typedef struct	s_fork
{
	t_mtx	fork;
	int		fork_id;
}					t_fork;

// PHILO
typedef struct	s_philo
{
	int			id;
	long		meals_counter;
	bool		full;
	long		last_meal_time; // temps passé depuis le dernier repas
	t_fork		*left_fork;
	t_fork		*right_fork;
	pthread_t	thread_id; // un philo = un thread
	t_table		*table;
}						t_philo;

struct	s_table
{
	long	philo_nbr;
	long	time_to_die;
	long	time_to_sleep;
	long	time_to_eat;
	long	nbr_limit_meals;
	long	start_simulation; // début des timestamps
	bool	end_simulation; // un philo meurs ou tout les philos sont full
	t_philo	*philos; // tab de philos
	t_fork	*forks; // tab de forks
};

// utils
int		print_error(const char *error);

// parsing
void	parse_input(t_table *table, char **av);

// init
void	data_init(t_table *table);
void	clean(t_table *table);

// simulation
void	dinner_start(t_table *table);

#endif
