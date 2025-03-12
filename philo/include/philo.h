/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 15:32:05 by clu               #+#    #+#             */
/*   Updated: 2025/03/12 11:23:32 by clu              ###   ########.fr       */
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

/* Status constants */
# define FORK_TAKEN 1
# define EATING 2
# define SLEEPING 3
# define THINKING 4
# define DIED 5

/* Forward declaration for circular reference */
typedef struct s_data	t_data;

/**
 * Structure to represent a philosopher
 */
typedef struct s_philo
{
	int				id;
	int				times_eaten;
	int				left_fork;
	int				right_fork;
	long long		last_meal;
	pthread_t		thread;
	pthread_mutex_t	meal_mutex;
	t_data			*data;
}	t_philo;

/**
 * Structure to store simulation data
 */
typedef struct s_data
{
	int				num_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				num_times_to_eat;
	bool			sim_stopped;
	long long		start_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mutex;
	pthread_t		monitor;
	t_philo			*philos;
    int             eating_philos;
    pthread_mutex_t host_mutex;
}	t_data;

/* Functions in main.c */
void		join_threads(t_data *data);
void		destroy_mutexes(t_data *data);
void		end_sim(t_data *data);

/* Functions in init_philo.c */
void		cleanup_forks(t_data *data, int i);
bool		init_forks(t_data *data);
bool		init_philo(t_philo *philo, int i, t_data *data);
bool		init_philos(t_data *data);
bool		start_sim(t_data *data);

/* Functions in philosopher.c */
void		*philo_routine(void *arg);
bool		take_forks(t_philo *philo);
bool		eat(t_philo *philo);
void		put_forks(t_philo *philo);
void		sleep_think(t_philo *philo);

/* Functions in monitor_philo.c */
void		*monitor_routine(void *arg);
bool		check_if_dead(t_philo *philo);
bool		check_if_full(t_data *data);
void		stop_sim(t_data *data);

/* Functions in utils.c */
long long	get_time(void);
void		ft_usleep(long long time);
void		print_status(t_philo *philo, int status);
void		error_exit(char *msg);

/* Functions in parser.c */
bool		parse_arguments(int argc, char **argv, t_data *data);
int			ft_atoi(const char *str);
bool		is_num(char *str);
int			print_usage(void);

#endif