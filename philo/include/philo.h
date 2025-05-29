/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 15:32:05 by clu               #+#    #+#             */
/*   Updated: 2025/05/30 00:38:52 by clu              ###   ########.fr       */
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
# define FORK		1
# define EATING 	2
# define SLEEPING 	3
# define THINKING 	4
# define DIED 		5

/* Declaration */
typedef struct s_data	t_data;
typedef struct s_philo	t_philo;

/* Fork struct */
typedef struct s_fork
{
	pthread_mutex_t	hold;
	_Atomic bool	free;
}	t_fork;

/* philosopher struct */
typedef struct s_data
{
	t_philo			*philos;
	t_fork			*forks;
	pthread_mutex_t	log_mutex;
	int				N_philos;
	int				full_count;
	long			sim_start;
	_Atomic bool	limit;
	_Atomic bool	stop_sim;
	_Atomic bool	died;
}	t_data;

typedef struct s_philo
{
	t_data			*data;
	t_fork			*l_fork;
	t_fork			*r_fork;
	pthread_t		thread;
	int				id;
	int				meal_count;
	int				meals_eaten;
	long			t_to_die;
	long			t_to_eat;
	long			t_to_sleep;
	long			last_meal;
	_Atomic bool	full;
}	t_philo;

/* Functions */
/* parse.c */
int		ft_atoi(const char *str);
bool	validate(char *arg);

/* init.c */
int		init_threads(t_data *data);
int		init_philos(t_philo *philos, int index, char **argv, t_data *data);
int		init_data(t_data *data);
int		set_philos(t_data *data, char **argv);
int		set_data(t_data *data, int argc, char **argv);

/* threads.c */
void	ft_usleep(t_philo *philos, long duration);
long	check_time(t_philo *philos, int	state);
void	print_state(t_philo *philos, int state);
void	start_meal(t_philo *philos);
int		thread_err(t_data *data, char *msg, int count);

/* threads_utils.c */
void	single_philo(t_philo *philos);
bool	try_l_fork(t_philo *philos);
bool	try_r_fork(t_philo *philos);
void	*monitor(void *arg);

/* routines.c */
void	eating(t_philo *philos);
void	sleeping(t_philo *philos);
void	thinking(t_philo *philos);
void	waiting(t_philo *philos);
void	*philo_routines(void *arg);

/* utils.c */
long	timestamp(t_data *data);
void	cleanup(t_data *data, int clean_mutex);
int		handle_err(t_data *data, char *msg, int cleanup);

#endif
