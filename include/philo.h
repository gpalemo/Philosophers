/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmauley <cmauley@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 21:16:36 by cmauley           #+#    #+#             */
/*   Updated: 2026/06/08 21:45:19 by cmauley          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <time.h> // gettimeofday
# include <sys/time.h> // struct timeval
# include <pthread.h> // mutex : init, destroy, lock, unlock | threads : create, join, detach
# include <string.h>
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
	int			meals_counter;
	int			full;
	long		last_meal_time; // temps passé depuis le dernier repas
	t_fork		*left_fork;
	t_fork		*right_fork;
	pthread_t	thread_id; // un philo = un thread
	t_table		*table;
}						t_philo;

struct	s_table
{
	int		philo_nbr;
	int		time_to_die;
	int		time_to_sleep;
	int		time_to_eat;
	int		nbr_limit_meals;
	long	start_simulation; // début des timestamps
	int		end_simulation; // un philo meurs ou tout les philos sont full
	t_philo	*philos; // tab de philos
	t_fork	*forks; // tab de forks
	t_mtx	print_mutex;
	int		forks_initialized;
	int		print_mutex_initialized;
};

// utils
int		print_error(const char *error);
void	*safe_malloc(size_t bytes);
int		safe_mutex_init(t_mtx *mutex);
int		safe_mutex_destroy(t_mtx *mutex);
int		safe_mutex_lock(t_mtx *mutex);
int		safe_mutex_unlock(t_mtx *mutex);
long	get_time(void);
void	clean(t_table *table);

// parsing
int	parse_input(t_table *table, char **av);

// init
int		data_init(t_table *table);

// simulation
int		dinner_start(t_table *table);
int		print_status(t_philo *philo, char *status);
void	*philo_routine(void *data);

#endif
