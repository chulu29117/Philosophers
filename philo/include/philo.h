/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 15:32:05 by clu               #+#    #+#             */
/*   Updated: 2025/05/28 03:00:30 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <limits.h>
# include <stdbool.h>

/* Status codes for print_status() */
# define FORK_TAKEN 1
# define EATING 2
# define SLEEPING 3
# define THINKING 4
# define DIED 5

/* Sleep interval for ft_usleep */
# define SLEEP_INTERVAL 100

/* Declaration */
typedef struct s_data t_data;

/* Philosopher struct */
typedef struct s_philo
{
	int				id;
	int				times_eaten;	// Number of times this philosopher has eaten
	int				left_fork;		// Index of the left fork in the forks array
	int				right_fork;		// Index of the right fork in the forks array
	long long 		last_meal;		// Timestamp of the last meal
	pthread_t		thread;			// Thread for this philosopher
	pthread_mutex_t	meal_mutex;		// Mutex to protect last_meal and times_eaten
	t_data			*data;
}	t_philo;

typedef struct s_data
{
	int				num_philos;			// Total number of philosophers
	int				time_to_die;		// Time in ms before a philosopher dies
	int				time_to_eat;		// Time in ms for a philosopher to eat
	int				time_to_sleep;		// Time in ms for a philosopher to sleep
	int				num_times_to_eat;	// Number of times each philosopher should eat
	bool			sim_stopped;		// Flag to indicate if the simulation has stopped
	long long		start_time;			// Timestamp when the simulation started
	bool			sim_duration;		// Flag to indicate if the simulation has a duration limit
	pthread_mutex_t	*forks;				// Array of mutexes for forks
	pthread_mutex_t	print_mutex;		// Mutex for printing philosopher status
	pthread_t		monitor;			// Thread for monitoring the simulation
	t_philo			*philo;				// Array of philosophers
	int				eating_philos;		// Number of philosophers currently eating
}	t_data;

/* Functions */
/* philo.c */
void		join_thread(t_data *data);
void		destroy_mutexes(t_data *data);
void		end_sim(t_data *data);

/* init_philo.c */
void		cleanup_forks(t_data *data, int i);
bool		init_forks(t_data *data);
bool		init_philo(t_philo *philo, int i, t_data *data);
bool		init_philos(t_data *data);
bool		start_sim(t_data *data);

/* philo_routine.c */
void		*philo_routine(void *arg);
bool		take_forks(t_philo *philo);
bool		eat(t_philo *philo);
void		put_forks(t_philo *philo);
void		sleep_think(t_philo *philo);
bool		host_try_eat(t_data *data);
void		host_done_eat(t_data *data);
bool		philo_cycle(t_philo *philo);
void		one_philo(t_philo *philo);

/* monitor.c */
void		*monitor_routine(void *arg);
bool		check_if_full(t_data *data);
bool		check_stop(t_data *data);
void		stop_sim(t_data *data);

/* utils.c */
long long	get_time(void);
void 		ft_usleep(long long duration_ms);
void		print_status(t_philo *philo, int status);

/* parsing.c */
bool		parse_arguments(int argc, char **argv, t_data *data);
int			ft_atoi(const char *str);
bool		is_num(char *str);
int			print_usage(void);

#endif
