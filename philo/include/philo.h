/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 15:32:05 by clu               #+#    #+#             */
/*   Updated: 2025/05/30 13:01:55 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef philo_H
# define philo_H

# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <limits.h>
# include <stdbool.h>

/*
** Usage message for invalid arguments
*/
# define USAGE "Usage: ./philo number_of_philosophers time_to_die time_to_eat \
time_to_sleep [number_of_times_each_philosopher_must_eat]\n"

/* Status codes for print_status() */
# define FORK	1
# define EATING 2
# define SLEEP 	3
# define THINK 	4
# define DIED 	5

/* Declaration */
typedef struct s_table	t_table;
typedef struct s_philo	t_philo;

/* Fork struct */
typedef struct s_fork
{
	pthread_mutex_t	hold;
	_Atomic bool	free;
}	t_fork;

/* philosopher struct */
typedef struct s_table
{
	t_philo			*philos;
	t_fork			*forks;
	pthread_mutex_t	print_mutex;
	_Atomic bool	limit;
	_Atomic bool	stop;
	_Atomic bool	died;
	int				n_philos;
	int				n_philos_full;
	long			start_time;
}	t_table;

typedef struct s_philo
{
	t_table			*table;
	t_fork			*l_fork;
	t_fork			*r_fork;
	pthread_t		thread;
	_Atomic bool	full;
	int				id;
	int				eat_count;
	long			t_to_die;
	long			t_to_eat;
	long			t_to_sleep;
	long			last_ate;
}	t_philo;

/* Functions */
/* parse.c */
int		ft_atoi(const char *str);
bool	validate(char *arg);

/* init.c */
int		init_threads(t_table *table);
int		init_philos(t_philo *philos, int index, char **argv, t_table *table);
int		init_table(t_table *table);
int		set_philos(t_table *table, char **argv);
int		set_table(t_table *table, int argc, char **argv);

/* threads.c */
void	ft_usleep(t_philo *philos, long duration);
long	check_death(t_philo *philos, int type);
void	print_state(t_philo *philos, int state);
void	start_eating(t_philo *philos);
int		thread_err(t_table *table, char *msg, int count);

/* threads_utils.c */
void	single_philo(t_philo *philos);
bool	try_l_fork(t_philo *philos);
bool	try_r_fork(t_philo *philos);
void	*monitor(void *arg);

/* routines.c */
void	eating(t_philo *philos);
void	sleeping(t_philo *philos);
void	thinking(t_philo *philos);
void	wait_start(t_philo *philos);
void	*philo_routines(void *arg);

/* utils.c */
long	timestamp(t_table *table);
void	cleanup(t_table *table, int clean_mutex);
int		handle_err(t_table *table, char *msg, int cleanup);

#endif
