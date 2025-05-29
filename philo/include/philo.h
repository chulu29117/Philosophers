/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 15:32:05 by clu               #+#    #+#             */
/*   Updated: 2025/05/29 17:00:15 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

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
# define USAGE_1 "Usage: ./philo number_of_philosophers" 
# define USAGE_2 " time_to_die time_to_eat time_to_sleep"
# define USAGE_3 " [number_of_times_each_philosopher_must_eat]\n"

/* Status codes for print_status() */
# define FORK_TAKEN 1
# define EATING 	2
# define SLEEPING 	3
# define THINKING 	4
# define DIED 		5

/* Sleep interval for ft_usleep */
# define SLEEP_INTERVAL	1000

/* Declaration */
typedef struct s_data	t_data;
typedef struct s_philo	t_philo;
typedef struct s_fork	t_fork;

/* Fork struct */
typedef struct s_fork
{
	pthread_mutex_t	lock;
}	t_fork;

/* Philosopher struct */
typedef struct s_philo
{
	int				id;
	int				meals;
	long			last_meal;
	pthread_t		thread;
	pthread_mutex_t	meal_mutex;
	t_data			*data;
}	t_philo;

typedef struct s_data
{
	int				num_philos;
	long			t_to_die;
	long			t_to_eat;
	long			t_to_sleep;
	int				max_meals;
	bool			stop;
	long			start;
	pthread_t		monitor_thread;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	stop_mutex;
	pthread_mutex_t	waiter;
	t_fork			*forks;
	t_philo			*philos;
}	t_data;

/* Functions */
/* parsing.c */
int			ft_atoi(const char *str);
bool		parse_args(int ac, char **av, t_data *data);

/* init.c */
bool		init_data(t_data *data);
bool		init_mutexes(t_data *data);
bool		init_philos(t_data *data);
bool		start_sim(t_data *data);
void		cleanup(t_data *data);

/* utils.c */
long		timestamp(void);
void		ft_usleep(long ms, t_philo *philos);
void		print_status(t_philo *philos, int start);
int			time_checks(t_philo *philos, int type);

/* routine.c */
void		*philo_routine(void *arg);

/* monitor.c */
void		*monitor_routine(void *arg);

#endif
